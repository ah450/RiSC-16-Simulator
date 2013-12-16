#ifndef INST_BUFFER_ROW_H
#define INST_BUFFER_ROW_H

class inst_buffer_row{
private:
	std::uint16_t instruction;
	unsigned int issue_at;
	unsigned int start_execute_at;
	unsigned int end_execute_at;
	unsigned int write_at;
	unsigned int commit_at;

public:
	virtual void record_issue_at(unsigned cycle);
	virtual void record_start_execute_at(unsigned cycle);
	virtual void record_end_execute_at(unsigned cycle);
	virtual void record_write_at(unsigned cycle);
	virtual void record_commit_at(unsigned cycle);
};

#endif