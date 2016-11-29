// Copyright (c) <2016>, <Nanyang Technological University> All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.

// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.

// 3. Neither the name of the copyright holder nor the names of its contributors
// may be used to endorse or promote products derived from this software without
// specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "localization.h"

Localization::Localization(int N) 
{
    solver = new Solver();

    solver->setBlockOrdering(false);

    se3blockSolver = new SE3BlockSolver(solver);

    optimizationsolver = new g2o::OptimizationAlgorithmLevenberg(se3blockSolver);

    optimizer.setAlgorithm(optimizationsolver);

    optimizer.setVerbose(true);

    iteration_max = 100;

    for (int i=0; i < N; ++i)
        robots.push_back(Robot(i));

    number_robots = N;
}


void Localization::solve()
{
    optimizer.initializeOptimization();

    optimizer.optimize(iteration_max);

    ROS_INFO("graph optimized!");
}


void Localization::addPoseEdge(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& pose_cov_)
{
    geometry_msgs::PoseWithCovarianceStamped pose_cov(*pose_cov_);

    g2o::EdgeSE3 *edge = new g2o::EdgeSE3();

    edge->vertices()[0] = optimizer.vertex(0);

    edge->vertices()[1] = optimizer.vertex(1);

    Eigen::Isometry3d measurement;

    tf::poseMsgToEigen(pose_cov.pose.pose, measurement);

    edge->setMeasurement(measurement);

    Eigen::Map<Eigen::MatrixXd> covariance(pose_cov.pose.covariance.data(), 6, 6);

    edge->setInformation(covariance.inverse());

    edge->setRobustKernel(new g2o::RobustKernelHuber());

    optimizer.addEdge(edge);

    // edges_pose.push_back(edge);

    ROS_INFO("added pose edge id: %d", pose_cov.header.seq);
}

//subscribe range msg from uwb_as node
void Localization::addRangeEdge(const uwb_driver::UwbRange::ConstPtr& uwb)
{
    //current index for the measurement
    int measCount = uwb->header.seq;

    //add two vertices for the new measurment
    int vertex0_id = number_robots*measCount+uwb->requester_idx;

    int vertex1_id = number_robots*measCount+uwb->responder_idx;

    g2o::VertexSE3* v0 = new g2o::VertexSE3();

    v0->setId(vertex0_id);

    //previous vertex estimate
    g2o::VertexSE3* v0_p = dynamic_cast<g2o::VertexSE3*>(optimizer.vertex(vertex0_id-number_robots));
    v0->setEstimate(v0_p->estimate());

    optimizer.addVertex(v0);

    // poses.push_back(v0);

    g2o::VertexSE3* v1 = new g2o::VertexSE3();

    v1->setId(vertex1_id);

    v1->setEstimate((dynamic_cast<g2o::VertexSE3*>(optimizer.vertex(vertex1_id-number_robots)))->estimate());

    optimizer.addVertex(v1);

    // poses.push_back(v1);

    //add edge
    g2o::EdgeSE3Range *edge = new g2o::EdgeSE3Range();

    edge->vertices()[0] = optimizer.vertex(vertex0_id);

    edge->vertices()[1] = optimizer.vertex(vertex1_id);

    edge->setMeasurement(uwb->distance);

    Eigen::MatrixXd information = Eigen::MatrixXd::Zero(1, 1);

    information(0,0) = 0.0025;

    edge->setInformation(information.inverse());

    edge->setRobustKernel(new g2o::RobustKernelHuber());

    optimizer.addEdge(edge);

    // edges_range.push_back(edge);
}