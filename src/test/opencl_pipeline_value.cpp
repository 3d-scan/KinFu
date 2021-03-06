#include <kinfu/opencl_pipeline_value.hpp>


#include <boost/compute.hpp>
#include <catch.hpp>


namespace {
	
	
	class mock final : public kinfu::opencl_pipeline_value<int> {
		
		
		private:
		
		
			int i_;
		
		
		public:
		
		
			using kinfu::opencl_pipeline_value<int>::opencl_pipeline_value;
			
			
			virtual const int & get () override {
				
				return i_;
				
			}
		
		
	};
	
	
	class fixture {
		
		
		protected:
		
		
			boost::compute::device dev;
			boost::compute::context ctx;
			boost::compute::command_queue q;
			mock pv;
			
			
		public:
		
		
			fixture () : dev(boost::compute::system::default_device()), ctx(dev), q(ctx,dev), pv(q) {	}
		
		
	};
	
	
}


SCENARIO_METHOD(fixture,"kinfu::opencl_pipeline_value tracks the association between a GPU pipeline value and its boost::compute::command_queue","[kinfu][pipeline_value][opencl_pipeline_value]") {
	
	GIVEN("A kinfu::opencl_pipeline_value created with some boost::compute::command_queue") {
		
		THEN("kinfu::opencl_pipeline_value::command_queue returns the same boost::compute::command_queue that was passed to the constructor") {
			
			CHECK(pv.command_queue()==q);
			
		}
		
		THEN("kinfu::opencl_pipeline_value::context returns the same boost::compute::context which was used to construct the boost::compute::command_queue passed to the constructor") {
			
			CHECK(pv.context()==ctx);
			
		}
		
		THEN("kinfu::opencl_pipeline_value::device returns the same boost::compute::device which was used to construct the boost::compute::command_queue passed to the constructor") {
			
			CHECK(pv.device()==dev);
			
		}
		
	}
	
}
