/***************************************************************************
MIT License

Copyright(c) 2023 lvchengTSH

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************/


// DOCUMENTATION
// 
// Dx12 hardware ray tracing library usage:
//		step 1. copy hwrtl.h, hwrtl_dx12.cpp to your project
//		step 2. enable graphics api by #define ENABLE_DX12_WIN 1
// 
// Vk hardware ray tracing libirary usage:
//		
// NOTICE:
//		1. we use space 0 for common srv / cbv / uav resource
//		2. use register space 1: for bindless vertex buffer and index buffer
//		3. use space 2 for gpu instance data srv buffer
//		3. use space 2 for root constant buffer
// 

#pragma once
#ifndef HWRTL_H
#define HWRTL_H

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>

#define ENABLE_DX12_WIN 1

namespace hwrtl
{
#define DEFINE_ENUM_FLAG_OPERATORS(ENUMTYPE) \
	inline           ENUMTYPE& operator|=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a | (__underlying_type(ENUMTYPE))b); } \
	inline           ENUMTYPE& operator&=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a & (__underlying_type(ENUMTYPE))b); } \
	inline           ENUMTYPE& operator^=(ENUMTYPE& a, ENUMTYPE b) { return a = (ENUMTYPE)((__underlying_type(ENUMTYPE))a ^ (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator| (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a | (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator& (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a & (__underlying_type(ENUMTYPE))b); } \
	inline constexpr ENUMTYPE  operator^ (ENUMTYPE  a, ENUMTYPE b) { return (ENUMTYPE)((__underlying_type(ENUMTYPE))a ^ (__underlying_type(ENUMTYPE))b); } \
	inline constexpr bool  operator! (ENUMTYPE  E)             { return !(__underlying_type(ENUMTYPE))E; } \
	inline constexpr ENUMTYPE  operator~ (ENUMTYPE  E)             { return (ENUMTYPE)~(__underlying_type(ENUMTYPE))E; }

	template<typename ENUMTYPE>
	constexpr bool EnumHasAnyFlags(ENUMTYPE enumValue, ENUMTYPE flag)
	{
		return ((__underlying_type(ENUMTYPE))enumValue & (__underlying_type(ENUMTYPE))flag) != 0;
	}

	template<typename T,int N, typename R>
	struct TVecN
	{
		TVecN()
		{
			for (uint32_t index = 0; index < N; index++)
			{
				(((T*)this)[index]) = 0;
			}
		}

		inline T operator[](uint32_t index) const
		{
			return (((T*)this)[index]);
		}

		inline R operator-() const
		{
			R r;
			for (int i = 0; i < N; ++i)
				((T*)&r)[i] = -((T*)this)[i];
			return r;
		}

		inline R operator*(T s) const
		{
			R r;
			for (int i = 0; i < N; ++i)
				((T*)&r)[i] = ((T*)this)[i] * s;
			return r;
		}

		template<typename T2, typename R2>
		inline T Dot(const TVecN<T2, N, R2>& o) const
		{
			T s = ((T*)this)[0] * o[0];
			for (int i = 1; i < N; ++i)
				s = s + (((T*)this)[i] * o[i]);
			return s;
		}

		template<typename T2, typename R2>
		inline void Set(const TVecN<T2, N,R2>& o)
		{
			for (int i = 0; i < N; ++i)
				((T*)this)[i] = static_cast<T>(((T2*)&o)[i]);
		}

#define VEC_OPERATOR(op) \
		inline R operator op(const TVecN<T,N,R>& o) const \
		{ \
			R r; \
			for (int i = 0; i < N; ++i) \
				((T*)(&r))[i] = ((T*)(this))[i] op ((T*)(&o))[i]; \
			return r; \
		} \

		VEC_OPERATOR(+);
		VEC_OPERATOR(-);
		VEC_OPERATOR(*);
		VEC_OPERATOR(/);
	};

	template<class F> 
	struct TVec2 : TVecN<F,2, TVec2<F>>
	{
		F x; F y;
		template<typename T2>
		TVec2(const TVec2<T2>& in) { TVecN<F, 2, TVec2<F>>::Set(in); }
		TVec2() :TVecN<F, 2, TVec2<F>>() {}
		TVec2(F x, F y) : x(x), y(y) {}
	};

	template<class F>
	struct TVec3 : TVecN<F, 3, TVec3<F>>
	{
		F x; F y; F z;
		template<typename T2>
		TVec3(const TVec3<T2>& in) { TVecN<F, 3, TVec3<F>>::Set(in); }
		TVec3() :TVecN<F, 3, TVec3<F>>() {}
		TVec3(F x, F y, F z): x(x), y(y),z(z){}
	};

	template<class F>
	struct TVec4 : TVecN<F, 4, TVec4<F>>
	{
		F x; F y; F z; F w;
		template<typename T2>
		TVec4(const TVec4<T2>& in) { TVecN<F, 4, TVec4<F>>::Set(in); }
		TVec4() :TVecN<F, 4, TVec4<F>>() {}
		TVec4(F x, F y, F z, F w) : x(x), y(y), z(z), w(w) {}
	};

	using Vec2 = TVec2<float>;
	using Vec2i = TVec2<int>;

	using Vec3 = TVec3<float>;
	using Vec3i = TVec3<int>;

	using Vec4 = TVec4<float>;
	using Vec4i = TVec4<int>;

	using WstringConverter = std::wstring_convert<std::codecvt_utf8<wchar_t>>;

	inline Vec3 NormalizeVec3(Vec3 vec);
	inline Vec3 CrossVec3(Vec3 A, Vec3 B)
	{
		return Vec3(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
	}

	struct Matrix44
	{
		union
		{
			float m[4][4];
			Vec4 row[4];
		};

		Matrix44();
		inline void SetIdentity();
		inline Matrix44 GetTrasnpose();
	};

	inline Matrix44 MatrixMulti(Matrix44 A, Matrix44 B);
	inline Matrix44 GetViewProjectionMatrixRightHand(Vec3 eyePosition, Vec3 eyeDirection, Vec3 upDirection, float fovAngleY, float aspectRatio, float nearZ, float farZ);

	// enum class 
	
	enum class EInstanceFlag
	{
		NONE,
		CULL_DISABLE,
		FRONTFACE_CCW,
	};

	enum class ERayShaderType
	{
		RAY_RGS, // ray gen shader
		RAY_MIH, // ray miss shader 
		RAY_AHS, // any hit shader
		RAY_CHS, // close hit shader

		RS_VS,	 // vertex shader
		RS_PS,	 // pixel shader
	};

	enum class EVertexFormat
	{
		FT_FLOAT3,
		FT_FLOAT2,
	};

	enum class ESlotType
	{
		ST_T = 0, // SRV
		ST_U, // UAV
		ST_B, // CBV
		ST_S, // Samper
	};

	enum class ETexFormat
	{
		FT_None,
		FT_DepthStencil,
		FT_RGBA8_UNORM, 
		FT_RGBA32_FLOAT,
	};

	enum class ETexUsage
	{
		USAGE_UAV = 1 << 1,
		USAGE_SRV = 1 << 2,
		USAGE_RTV = 1 << 3,
		USAGE_DSV = 1 << 4,
	};
	DEFINE_ENUM_FLAG_OPERATORS(ETexUsage);

	enum class EBufferUsage : uint32_t
	{
		USAGE_VB = (1 << 0) , // vertex buffer
		USAGE_IB = (1 << 1), // index buffer
		USAGE_CB = (1 << 2), // constant buffer
		USAGE_Structure = (1 << 3), // structure buffer
		USAGE_BYTE_ADDRESS = (1 << 4)  // byte address buffer for bindless vb ib
	};
	DEFINE_ENUM_FLAG_OPERATORS(EBufferUsage);

	// struct 

	struct SMeshInstanceInfo
	{
		float m_transform[3][4];
		EInstanceFlag m_instanceFlag;
		uint32_t m_instanceID = 0;
		SMeshInstanceInfo();
	};

	struct SShader
	{
		ERayShaderType m_eShaderType;
		const std::wstring m_entryPoint;
	};

	struct STextureCreateDesc
	{
		ETexUsage m_eTexUsage;
		ETexFormat m_eTexFormat;
		uint32_t m_width;
		uint32_t m_height;
		const uint8_t* m_srcData;
	};

	struct SShaderResources
	{
		uint32_t m_nSRV = 0;
		uint32_t m_nUAV = 0;
		uint32_t m_nCBV = 0;
		uint32_t m_nSampler = 0;
		uint32_t m_rootConstant = 0;

		uint32_t m_useBindlessTex2D = false;
		uint32_t m_useByteAddressBuffer = false;

		uint32_t operator[](std::size_t index)
		{
			return *((uint32_t*)(this) + index);
		}
	};

	// RHI Object Define

	class CGraphicsPipelineState 
	{
	public:
		CGraphicsPipelineState() {}
		virtual ~CGraphicsPipelineState() {}
	};
	
	class CRayTracingPipelineState 
	{
	public:
		CRayTracingPipelineState() {}
		virtual ~CRayTracingPipelineState() {}
	};

	class CTexture2D 
	{
	public:
		CTexture2D() {}
		virtual ~CTexture2D() {}
		virtual uint32_t GetOrAddTexBindlessIndex() = 0;

		uint32_t m_texWidth;
		uint32_t m_texHeight;
	};

	class CBuffer
	{
	public:
		CBuffer() {}
		virtual ~CBuffer() {}
		virtual uint32_t GetOrAddByteAddressBindlessIndex() = 0;
	};

	class CBottomLevelAccelerationStructure
	{
	public:
		virtual ~CBottomLevelAccelerationStructure() {}
	};

	class CTopLevelAccelerationStructure
	{
	public:
		virtual ~CTopLevelAccelerationStructure() {}
	};

	struct SCPUMeshData
	{
		const Vec3* m_pPositionData = nullptr;
		const void* m_pIndexData = nullptr; // optional

		const Vec2* m_pUVData = nullptr; // optional
		const Vec3* m_pNormalData = nullptr; // optional

		uint32_t m_nIndexStride = 0;

		uint32_t m_nVertexCount = 0;
		uint32_t m_nIndexCount = 0;

		std::vector<SMeshInstanceInfo>instanes;
	};

	struct SGpuBlasData
	{
		uint32_t m_nVertexCount = 0;
		uint32_t m_nIndexStride = 0;
		std::vector<SMeshInstanceInfo>instanes;

		std::shared_ptr<CBuffer>m_pVertexBuffer;
		std::shared_ptr<CBuffer>m_pIndexBuffer;
		std::shared_ptr<CBottomLevelAccelerationStructure>m_pBLAS;
	};

	struct SShaderDefine
	{
		std::wstring m_defineName;
		std::wstring m_defineValue;
	};

	struct SRayTracingPSOCreateDesc
	{
		std::wstring filename; 
		std::vector<SShader>rtShaders; 
		uint32_t maxTraceRecursionDepth; 
		SShaderResources rayTracingResources; 
		SShaderDefine* shaderDefines; 
		uint32_t defineNum;
	};

	struct SRasterizationPSOCreateDesc
	{
		std::wstring filename; 
		std::vector<SShader>rtShaders; 
		SShaderResources rasterizationResources; 
		std::vector<EVertexFormat>vertexLayouts; 
		std::vector<ETexFormat>rtFormats; 
		ETexFormat dsFormat;
	};

	class CDeviceCommand
	{
	public:
		virtual void OpenCmdList() = 0;
		virtual void CloseAndExecuteCmdList() = 0;
		virtual void WaitGPUCmdListFinish() = 0;
		virtual void ResetCmdAlloc() = 0;

		virtual std::shared_ptr<CRayTracingPipelineState> CreateRTPipelineStateAndShaderTable(SRayTracingPSOCreateDesc& rtPsoDesc) = 0;
		virtual std::shared_ptr<CGraphicsPipelineState>  CreateRSPipelineState(SRasterizationPSOCreateDesc& rsPsoDesc) = 0;

		virtual std::shared_ptr<CTexture2D> CreateTexture2D(STextureCreateDesc texCreateDesc) = 0;
		virtual std::shared_ptr<CBuffer> CreateBuffer(const void* pInitData, uint64_t nByteSize, uint64_t nStride, EBufferUsage bufferUsage) = 0;

		virtual void BuildBottomLevelAccelerationStructure(std::vector<std::shared_ptr<SGpuBlasData>>& inoutGPUMeshDataPtr) = 0;
		virtual std::shared_ptr<CTopLevelAccelerationStructure> BuildTopAccelerationStructure(std::vector<std::shared_ptr<SGpuBlasData>>& gpuMeshData) = 0;

		virtual void* LockTextureForRead(std::shared_ptr<CTexture2D> readBackTexture) = 0;
		virtual void UnLockTexture(std::shared_ptr<CTexture2D> readBackTexture) = 0;
	};

	class CContext
	{
	public:
		CContext() {}
		virtual ~CContext() {}
	};

	class CRayTracingContext : public CContext
	{
	public:
		CRayTracingContext() {}
		virtual ~CRayTracingContext() {}

		virtual void BeginRayTacingPasss() = 0;
		virtual void EndRayTacingPasss() = 0;

		virtual void SetRayTracingPipelineState(std::shared_ptr<CRayTracingPipelineState>rtPipelineState) = 0;

		virtual void SetTLAS(std::shared_ptr<CTopLevelAccelerationStructure> tlas, uint32_t bindIndex) = 0;
		virtual void SetShaderSRV(std::shared_ptr<CTexture2D>tex2D, uint32_t bindIndex) = 0;
		virtual void SetShaderSRV(std::shared_ptr<CBuffer>buffer, uint32_t bindIndex) = 0;
		virtual void SetShaderUAV(std::shared_ptr<CTexture2D>tex2D, uint32_t bindIndex) = 0;

		virtual void SetRootConstants(uint32_t bindIndex, uint32_t num32BitValuesToSet, const void* srcData, uint32_t destRootConstantOffsets) = 0;

		virtual void SetConstantBuffer(std::shared_ptr<CBuffer> constantBuffer, uint32_t bindIndex) = 0;

		virtual void DispatchRayTracicing(uint32_t width, uint32_t height) = 0;
	};

	class CGraphicsContext : public CContext
	{
	public:
		CGraphicsContext() {}
		virtual ~CGraphicsContext() {}

		virtual void BeginRenderPasss() = 0;
		virtual void EndRenderPasss() = 0;

		virtual void SetGraphicsPipelineState(std::shared_ptr<CGraphicsPipelineState>rtPipelineState) = 0;

		virtual void SetViewport(float width, float height) = 0;

		virtual void SetRenderTargets(std::vector<std::shared_ptr<CTexture2D>> renderTargets, std::shared_ptr<CTexture2D> depthStencil = nullptr, bool bClearRT = true, bool bClearDs = true) = 0;
		
		virtual void SetShaderSRV(std::shared_ptr<CTexture2D>tex2D, uint32_t bindIndex) = 0;
		virtual void SetConstantBuffer(std::shared_ptr<CBuffer> constantBuffer,uint32_t bindIndex) = 0;
		
		virtual void SetVertexBuffers(std::vector<std::shared_ptr<CBuffer>> vertexBuffers) = 0;

		virtual void DrawInstanced(uint32_t vertexCountPerInstance, uint32_t InstanceCount, uint32_t StartVertexLocation, uint32_t StartInstanceLocation) = 0;
	};

	struct SDeviceInitConfig {
		std::string m_pixCaptureDllPath;
		std::string m_pixCaptureSavePath;
	};

	void Init(const SDeviceInitConfig deviceInitConfig = SDeviceInitConfig());
	void Shutdown();

	std::shared_ptr <CDeviceCommand> CreateDeviceCommand();
	std::shared_ptr<CRayTracingContext> CreateRayTracingContext();
	std::shared_ptr<CGraphicsContext> CreateGraphicsContext();

	inline Vec3 NormalizeVec3(Vec3 vec)
	{
		float lenght = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		if (lenght > 0.0)
		{
			lenght = 1.0f / lenght;
		}
		return Vec3(vec.x, vec.y, vec.z) * lenght;
	}

	inline Matrix44::Matrix44()
	{
		for (uint32_t i = 0; i < 4; i++)
			for (uint32_t j = 0; j < 4; j++)
				m[i][j] = 0;
	}

	inline void Matrix44::SetIdentity()
	{
		row[0] = Vec4(1, 0, 0, 0);
		row[1] = Vec4(0, 1, 0, 0);
		row[2] = Vec4(0, 0, 1, 0);
		row[3] = Vec4(0, 0, 0, 1);
	}

	inline Matrix44 Matrix44::GetTrasnpose()
	{
		Matrix44 C;
		for (uint32_t i = 0; i < 4; i++)
			for (uint32_t j = 0; j < 4; j++)
				C.m[i][j] = m[j][i];
		return C;
	}

	inline Matrix44 MatrixMulti(Matrix44 A, Matrix44 B)
	{
		Matrix44 C;
		for (uint32_t i = 0; i < 4; i++)
		{
			for (uint32_t j = 0; j < 4; j++)
			{
				C.m[i][j] = A.row[i].Dot(Vec4(B.m[0][j], B.m[1][j], B.m[2][j], B.m[3][j]));
			}
		}
		return C;
	}

	inline Matrix44 GetViewProjectionMatrixRightHand(
		Vec3 eyePosition, Vec3 eyeDirection, Vec3 upDirection, float fovAngleY, float aspectRatio, float nearZ, float farZ)
	{
		//reverse z
		float tempZ = nearZ;
		nearZ = farZ;
		farZ = tempZ;

		//wolrd space			 
		//	  z y				 
		//	  |/				 
		//    --->x				 

		//capera space
		//	  y
		//	  |
		//    ---->x
		//	 /
		//  z

		Vec3 zAxis = NormalizeVec3(-eyeDirection);
		Vec3 xAxis = NormalizeVec3(CrossVec3(upDirection, zAxis));
		Vec3 yAxis = CrossVec3(zAxis, xAxis);

		Vec3 negEye = -eyePosition;

		Matrix44 camTranslate;
		camTranslate.SetIdentity();
		camTranslate.m[0][3] = negEye.x;
		camTranslate.m[1][3] = negEye.y;
		camTranslate.m[2][3] = negEye.z;

		Matrix44 camRotate;
		camRotate.row[0] = Vec4(xAxis.x, xAxis.y, xAxis.z, 0);
		camRotate.row[1] = Vec4(yAxis.x, yAxis.y, yAxis.z, 0);
		camRotate.row[2] = Vec4(zAxis.x, zAxis.y, zAxis.z, 0);
		camRotate.row[3] = Vec4(0, 0, 0, 1);

		Matrix44 viewMat = MatrixMulti(camRotate, camTranslate);

		float radians = 0.5f * fovAngleY * 3.1415926535f / 180.0f;
		float sinFov = std::sin(radians);
		float cosFov = std::cos(radians);

		float Height = cosFov / sinFov;
		float Width = Height / aspectRatio;
		float fRange = farZ / (nearZ - farZ);

		Matrix44 projMat;
		projMat.m[0][0] = Width;
		projMat.m[1][1] = Height;
		projMat.m[2][2] = fRange;
		projMat.m[2][3] = fRange * nearZ;
		projMat.m[3][2] = -1.0f;
		return MatrixMulti(projMat, viewMat);
	}

	inline SMeshInstanceInfo::SMeshInstanceInfo()
	{
		for (int i = 0; i < 12; i++)
		{
			((float*)m_transform)[i] = 0;
		}
		m_transform[0][0] = 1;
		m_transform[1][1] = 1;
		m_transform[2][2] = 1;

		m_instanceFlag = EInstanceFlag::NONE;
	}
}

#endif
