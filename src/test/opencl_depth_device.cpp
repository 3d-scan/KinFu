#include <kinfu/opencl_depth_device.hpp>


#include <boost/compute.hpp>
#include <Eigen/Dense>
#include <kinfu/mock_depth_device.hpp>
#include <kinfu/opencl_vector_pipeline_value.hpp>
#include <algorithm>
#include <vector>
#include <catch.hpp>


SCENARIO("kinfu::opencl_depth_device objects wrap a kinfu::depth_device and upload its depth frames to the GPU","[kinfu][depth_device][opencl_depth_device]") {
	
	GIVEN("A kinfu::opencl_depth_device which wraps a kinfu::depth_device which does not upload its depth frames to the GPU") {
		
		kinfu::mock_depth_device mdd(Eigen::Matrix3f::Zero(),2,2);
		std::vector<float> v{1.0f,2.0f,3.0f,4.0f};
		mdd.add(v);	//	Copy so we have something to compare against
		auto dev=boost::compute::system::default_device();
		boost::compute::context ctx(dev);
		boost::compute::command_queue q(ctx,dev);
		kinfu::opencl_depth_device gpudd(mdd,q);
		
		WHEN("It is invoked") {
			
			auto pv=gpudd();
			
			THEN("The returned kinfu::pipeline_value is-a kinfu::opencl_vector_pipeline_value") {
				
				auto ptr=dynamic_cast<kinfu::opencl_vector_pipeline_value<float> *>(pv.get());
				REQUIRE(ptr);
				
				AND_THEN("The frame is available on the GPU") {
					
					auto && ve=ptr->vector();
					q.finish();	//	In case any operation is still pending
					CHECK(std::equal(v.begin(),v.end(),ve.begin(),ve.end()));
					
				}
				
			}
			
			THEN("The frame is available on the CPU") {
				
				auto && ve=pv->get();
				CHECK(std::equal(v.begin(),v.end(),ve.begin(),ve.end()));
				
			}
			
		}
		
	}
	
}
