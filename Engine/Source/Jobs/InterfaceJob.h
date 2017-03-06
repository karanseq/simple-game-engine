#ifndef INTERFACE_JOB_H_
#define INTERFACE_JOB_H_

// engine includes
#include "Data\PooledString.h"

namespace engine {
namespace jobs {

class InterfaceJob
{
public:
	InterfaceJob() {};
	virtual ~InterfaceJob() {};

	virtual void DoWork() = 0;

	inline const engine::data::PooledString& GetName() const { return name_; }
	inline void SetName(const engine::data::PooledString& i_name) { name_ = i_name; }

protected:
	engine::data::PooledString name_;

}; // class InterfaceJob

} // namespace jobs
} // namespace engine

#endif // INTERFACE_JOB_H_