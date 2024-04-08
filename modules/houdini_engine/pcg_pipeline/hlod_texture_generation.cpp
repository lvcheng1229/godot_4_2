#include "hlod_texture_generation.h"
#include "editor/editor_settings.h"
#include "core/templates/hash_map.h"
#include "scene/resources/surface_tool.h"

using namespace hwrtl::hlod;

__pragma(optimize("", off))

void HLODTexGenerator::init() {
}

static std::shared_ptr<hwrtl::CTexture2D> create_high_poly_input_texture(Ref<Texture2D> tex_ref) {
	Ref<Image> img_ref = tex_ref->get_image();
	Ref<Image> new_img = img_ref->get_image_from_mipmap(0);
	new_img->convert(Image::Format::FORMAT_RGBA8);
	new_img->get_data();

	Vector<uint8_t> r_orm = new_img->get_data();

	hwrtl::STextureCreateDesc texDesc;
	texDesc.m_width = new_img->get_width();
	texDesc.m_height = new_img->get_height();
	texDesc.m_eTexFormat = hwrtl::ETexFormat::FT_RGBA8_UNORM;
	texDesc.m_eTexUsage = hwrtl::ETexUsage::USAGE_SRV;
	texDesc.m_srcData = r_orm.ptr();
	return CreateHighPolyTexture2D(texDesc);
}

static std::shared_ptr<hwrtl::CTexture2D> find_or_create_high_input_texture(Ref<BaseMaterial3D> mat_ref, BaseMaterial3D::TextureParam tex_param, HashMap<Texture2D *, std::shared_ptr<hwrtl::CTexture2D>>& image_set) {
	Ref<Texture2D> tex_ref = mat_ref->get_texture(tex_param);

	if (image_set.find(tex_ref.ptr()) == image_set.end()) {
		image_set.insert(tex_ref.ptr(), create_high_poly_input_texture(tex_ref));
	}

	return image_set[tex_ref.ptr()];
}

void HLODTexGenerator::hlod_mesh_texture_generate(const Vector<HlodInputMesh> &input_meshs, const HlodSimplifiedMesh &hlod_mesh_simplified, Ref<ArrayMesh> &out_mesh) {
	String houdini_module_path = EDITOR_GET("filesystem/houdini/HoudiniModulePath");

	String shader_path = houdini_module_path + "/hwrtl/";
	CharString shader_path_char = shader_path.utf8();

	String pix_dll_path = houdini_module_path + "/gfx_cap_dll/WinPixGpuCapturer.dll";
	CharString pix_dll_path_char = pix_dll_path.utf8();

	hwrtl::SDeviceInitConfig device_init_cfg;
	device_init_cfg.m_pixCaptureDllPath = pix_dll_path_char;
	device_init_cfg.m_pixCaptureSavePath = pix_dll_path_char;

	SHLODConfig hlod_fonfig;
	hlod_fonfig.m_sShaderPath = shader_path_char.get_data();
	hlod_fonfig.m_nShaderPathLength = shader_path_char.length();
	hlod_fonfig.m_nHLODTextureSize = hwrtl::Vec2i(1024, 1024);
	InitHLODTextureBaker(hlod_fonfig, device_init_cfg);

	Vector<Vector2> uvs;
	for (int index = 0; index < hlod_mesh_simplified.points.size(); index++) {
		Vector2 uv = Vector2(hlod_mesh_simplified.uv.get(index).x, hlod_mesh_simplified.uv.get(index).y);
		uvs.push_back(uv);
	}

	SHLODBakerMeshDesc bakeMeshDesc;
	bakeMeshDesc.m_pPositionData = (hwrtl::Vec3*)hlod_mesh_simplified.points.ptr();
	bakeMeshDesc.m_pUVData = (hwrtl::Vec2 *)uvs.ptr();
	bakeMeshDesc.m_nVertexCount = hlod_mesh_simplified.points.size();
	SetHLODMesh(bakeMeshDesc);

	HashMap<Texture2D *, std::shared_ptr<hwrtl::CTexture2D>> image_set;
	Vector<Vector<Vector3>> vertices_array;
	Vector<Vector<Vector2>> uv_array;

	std::vector<SHLODHighPolyMeshDesc> high_poly_mesh_descs;

	int array_size = 0;
	for (int index = 0; index < input_meshs.size(); index++) {
		Ref<Mesh> mesh_ref = input_meshs[index].mesh;
		for (int i = 0; i < mesh_ref->get_surface_count(); i++) {
			array_size++;
		}
	}

	vertices_array.resize(array_size);
	uv_array.resize(array_size);

	int mesh_index = 0;
	for (int index = 0; index < input_meshs.size(); index++)
	{
		Ref<Mesh> mesh_ref = input_meshs[index].mesh;
		for (int i = 0; i < mesh_ref->get_surface_count(); i++) {
			if (mesh_ref->surface_get_primitive_type(i) != Mesh::PRIMITIVE_TRIANGLES) {
				continue;
			}
			
			Ref<BaseMaterial3D> mat_ref = mesh_ref->surface_get_material(i);

			std::shared_ptr<hwrtl::CTexture2D> albdedo_tex = find_or_create_high_input_texture(mat_ref, BaseMaterial3D::TEXTURE_ALBEDO, image_set);
			std::shared_ptr<hwrtl::CTexture2D> metallic_tex = find_or_create_high_input_texture(mat_ref, BaseMaterial3D::TEXTURE_METALLIC, image_set);
			std::shared_ptr<hwrtl::CTexture2D> normal_tex = find_or_create_high_input_texture(mat_ref, BaseMaterial3D::TEXTURE_NORMAL, image_set);

			Array a = mesh_ref->surface_get_arrays(i);
			Vector<Vector3> vertices = a[Mesh::ARRAY_VERTEX];
			Vector<Vector2> uv = a[Mesh::ARRAY_TEX_UV];

			Vector<int> vertex_index = a[Mesh::ARRAY_INDEX];

			const Vector3 *vr = vertices.ptr();
			const Vector2 *uvr = uv.ptr();

			int facecount;
			const int *ir = nullptr;

			if (vertex_index.size()) {
				facecount = vertex_index.size() / 3;
				ir = vertex_index.ptr();
			} else {
				facecount = vertices.size() / 3;
			}

			Vector<Vector3> converted_mesh_pos;
			Vector<Vector2> converted_mesh_uv;

			for (int j = 0; j < facecount; j++) {
				uint32_t vidx[3];

				if (ir) {
					for (int k = 0; k < 3; k++) {
						vidx[k] = ir[j * 3 + k];
					}
				} else {
					for (int k = 0; k < 3; k++) {
						vidx[k] = j * 3 + k;
					}
				}

				for (int k = 0; k < 3; k++) {
					const Transform3D transform = input_meshs[index].xform;
					Vector3 v = transform.xform(vr[vidx[k]]);
					converted_mesh_pos.push_back(v);
					converted_mesh_uv.push_back(Vector2(uvr[vidx[k]].x, uvr[vidx[k]].y));
				}
			}

			vertices_array.set(mesh_index, converted_mesh_pos);
			uv_array.set(mesh_index, converted_mesh_uv);

			SHLODHighPolyMeshDesc highPolyMesh;
			highPolyMesh.m_pPositionData = (const hwrtl::Vec3 *)vertices_array.get(mesh_index).ptr();
			highPolyMesh.m_pUVData = (const hwrtl::Vec2 *)uv_array.get(mesh_index).ptr();
			highPolyMesh.m_nVertexCount = vertices_array.size();
			highPolyMesh.m_meshInstanceInfo = hwrtl::SMeshInstanceInfo();
			highPolyMesh.m_meshInstanceInfo.m_instanceFlag = hwrtl::EInstanceFlag::CULL_DISABLE;
			highPolyMesh.m_pBaseColorTexture = albdedo_tex;
			highPolyMesh.m_pMetallicTexture = metallic_tex;
			highPolyMesh.m_pNormalTexture = normal_tex;
			high_poly_mesh_descs.push_back(highPolyMesh);
			mesh_index++;
		}
	}

	SetHighPolyMesh(high_poly_mesh_descs);
	BakeHLODTexture();

	SHLODTextureOutData hlodTextureData;
	GetHLODTextureData(hlodTextureData);
	
	{
		Ref<SurfaceTool> surftool;
		surftool.instantiate();
		surftool->begin(Mesh::PRIMITIVE_TRIANGLES);

		for (int index = 0; index < hlod_mesh_simplified.points.size(); index++) {
			surftool->set_normal(hlod_mesh_simplified.normal.get(index));

			Vector2 uv = Vector2(hlod_mesh_simplified.uv.get(index).x, hlod_mesh_simplified.uv.get(index).y);
			surftool->set_uv(uv);

			surftool->add_vertex(hlod_mesh_simplified.points.get(index));
		}

		for (int index = 0; index < hlod_mesh_simplified.indices.size(); index++) {
			surftool->add_index(hlod_mesh_simplified.indices.get(index));
		}

		Ref<StandardMaterial3D> mat = memnew(StandardMaterial3D);
		int tex_size = hlodTextureData.m_texSize.x * hlodTextureData.m_texSize.y * 4;
		Vector<uint8_t> data;
		data.resize(tex_size);
		uint8_t *w = data.ptrw();
		{
			memcpy(w, hlodTextureData.destBaseColorOutputData, tex_size);
			Ref<Image> result_albedo_image = memnew(Image);
			result_albedo_image->create_from_data(hlodTextureData.m_texSize.x, hlodTextureData.m_texSize.y, false, Image::FORMAT_RGBA8, data);
			mat->set_texture(BaseMaterial3D::TEXTURE_ALBEDO, result_albedo_image);
		}

		{
			Ref<Image> result_metallic_image = memnew(Image);
			result_metallic_image->create_from_data(hlodTextureData.m_texSize.x, hlodTextureData.m_texSize.y, false, Image::FORMAT_RGBA8, data);
			mat->set_texture(BaseMaterial3D::TEXTURE_METALLIC, result_metallic_image);
		}

		{
			Ref<Image> result_normal_image = memnew(Image);
			result_normal_image->create_from_data(hlodTextureData.m_texSize.x, hlodTextureData.m_texSize.y, false, Image::FORMAT_RGBA8, data);
			mat->set_texture(BaseMaterial3D::TEXTURE_NORMAL, result_normal_image);
		}
		
		surftool->set_material(mat);

		Ref<ArrayMesh> array_mesh = surftool->commit();
		out_mesh = array_mesh;
	}

	FreeHloadTextureData(hlodTextureData);
	DeleteHLODBaker();
	
}
