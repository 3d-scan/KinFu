#include <dynfu/kinect_fusion_opencl_frame_to_frame_surface_prediction_pipeline_block.hpp>


#include <boost/compute.hpp>
#include <dynfu/cpu_pipeline_value.hpp>
#include <dynfu/filesystem.hpp>
#include <dynfu/path.hpp>
#include <Eigen/Dense>
#include <algorithm>
#include <tuple>
#include <vector>
#include <catch.hpp>


namespace {


	class fixture {

		private:

			static dynfu::filesystem::path cl_path () {

				dynfu::filesystem::path retr(dynfu::current_executable_parent_path());
				retr/="..";
				retr/="cl";

				return retr;

			}

		protected:

			boost::compute::device dev;
			boost::compute::context ctx;
			boost::compute::command_queue q;

		public:

		fixture () : dev(boost::compute::system::default_device()), ctx(dev), q(ctx,dev) {	}

	};


}


SCENARIO_METHOD(fixture,"dynfu::kinect_fusion_opencl_frame_to_frame_surface_prediction_pipeline_block objects simply copy the provided vertex and normal maps to their output","[dynfu][surface_prediction_pipeline_block][kinect_fusion_opencl_frame_to_frame_surface_prediction_pipeline_block]") {

	GIVEN("A dynfu::kinect_fusion_opencl_frame_to_frame_surface_prediction_pipeline_block object") {

		dynfu::kinect_fusion_opencl_frame_to_frame_surface_prediction_pipeline_block sppb(q);

		WHEN("It is invoked") {

			dynfu::cpu_pipeline_value<std::vector<float>> tsdf;
			tsdf.emplace();
			dynfu::cpu_pipeline_value<Eigen::Matrix4f> pose;
			pose.emplace(Eigen::Matrix4f::Zero());
			dynfu::cpu_pipeline_value<std::vector<Eigen::Vector3f>> vpv;
			dynfu::cpu_pipeline_value<std::vector<Eigen::Vector3f>> npv;
			auto && v=vpv.emplace();
			auto && n=npv.emplace();
			v.emplace_back(1.0f,2.0f,3.0f);
			n.emplace_back(4.0f,5.0f,6.0f);

			auto tuple=sppb(tsdf,0,0,0,pose,Eigen::Matrix3f::Zero(),vpv,npv,dynfu::kinect_fusion_opencl_frame_to_frame_surface_prediction_pipeline_block::value_type{});

			THEN("The vertex and normal maps provided are simply copied into the result") {

				auto && vr=std::get<0>(tuple)->get();
				CHECK(std::equal(vr.begin(),vr.end(),v.begin(),v.end()));
				auto && nr=std::get<1>(tuple)->get();
				CHECK(std::equal(nr.begin(),nr.end(),n.begin(),n.end()));

			}

		}

	}

}