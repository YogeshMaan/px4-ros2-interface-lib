/****************************************************************************
 * Copyright (c) 2023 PX4 Development Team.
 * SPDX-License-Identifier: BSD-3-Clause
 ****************************************************************************/

#pragma once

#include <px4_msgs/msg/actuator_motors.hpp>
#include <px4_msgs/msg/actuator_servos.hpp>
#include <Eigen/Core>

#include <px4_sdk/common/setpoint_base.hpp>

namespace px4_sdk
{

class DirectActuatorsSetpointType : public SetpointBase
{
public:
  explicit DirectActuatorsSetpointType(
    rclcpp::Node & node,
    const std::string & topic_namespace_prefix = "");

  static constexpr int kMaxNumMotors = px4_msgs::msg::ActuatorMotors::NUM_CONTROLS;
  static constexpr int kMaxNumServos = px4_msgs::msg::ActuatorServos::NUM_CONTROLS;

  ~DirectActuatorsSetpointType() override = default;

  Configuration getConfiguration() override;
  float desiredUpdateRateHz() override {return 500.F;}

  /**
   * Send servos setpoint
   * @param servo_commands range: [-1, 1], where 1 means maximum positive position,
   *                -1 maximum negative and NaN maps to disarmed
   */
  void updateServos(const Eigen::Vector<float, kMaxNumServos> & servo_commands);

  /**
   * Send motors setpoint
   * @param motor_commands range: [-1, 1], where 1 means maximum positive thrust,
   *                -1 maximum negative (if not supported by the output, <0 maps to NaN),
   *                and NaN maps to disarmed (stop the motors)
   */
  void updateMotors(const Eigen::Vector<float, kMaxNumMotors> & motor_commands);

private:
  rclcpp::Node & _node;
  rclcpp::Publisher<px4_msgs::msg::ActuatorMotors>::SharedPtr _actuator_motors_pub;
  rclcpp::Publisher<px4_msgs::msg::ActuatorServos>::SharedPtr _actuator_servos_pub;
};

} /* namespace px4_sdk */