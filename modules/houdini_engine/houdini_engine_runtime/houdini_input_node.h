#pragma once
class HoudiniInputNode {
public:
	void set_input_node_id(int node_id) { input_node_id = node_id; };
	inline int get_input_node_id() const{ return input_node_id; }
private:
	int input_node_id;
};
