#include "messages.h"
#include "serialize.h"
#include <sstream>

Test_Inbound::Test_Inbound(const Packet &p)
{
	std::tie(
		field_1, field_2) = deserialize<float, float>(p.data());
}

Packet Test_Outbound::pack()
{
	return Packet(
		Test_Outbound::id,
		serialize<
			float, float, float>(std::make_tuple(
			field_1, field_2, field_3)));
}

Position::Position(const Packet &p)
{
	std::tie(
		x_trans, y_trans, z_trans,
		x_rot, y_rot, z_rot,
		w,
		frame_number) = deserialize<float, float, float,
									float, float, float,
									float,
									int32_t>(p.data());
}

Simple_Move::Simple_Move(const Packet &p)
{
	std::tie(
		distance, curvature, velocity, trans_id) = deserialize<float, float, float, int32_t>(p.data());
}

Packet Move_Feedback::pack()
{
	return Packet(
		Move_Feedback::id,
		serialize<
			float, float, float, int32_t>(std::make_tuple(
			distance, curvature, velocity, trans_id)));
}

Twist::Twist()
{
}

Twist::Twist(const Packet &p)
{
	std::tie(linear, angular) = deserialize<float, float>(p.data());
}

Packet Twist::pack()
{
	return Packet(
		Twist::id,
		serialize<
			float, float>(std::make_tuple(
			linear, angular)));
}

std::string Twist::repr()
{
	std::stringstream ss;
	ss << "Twist<linear: " << linear << " angular: " << angular << ">\n";
	return ss.str();
}

// Move::Move()
// {
// }

// Move::Move(const Packet &p)
// {
// 	std::tie(power) = deserialize<float>(p.data());
// }

// Packet Move::pack()
// {
// 	return Packet(
// 		Move::id,
// 		serialize<
// 			float>(std::make_tuple(
// 			power)));
// }

// std::string Move::repr()
// {
// 	std::stringstream ss;
// 	ss << "Move, Power: " << power << ">\n";
// 	return ss.str();
// }

// Turn::Turn(){

// }

// Turn::Turn(const Packet& p){
// 	std::tie(left, right) = deserialize<float, float>(p.data());
// }

// Packet Turn::pack(){
// 		return Packet(
// 		Turn::id,
// 		serialize<
// 			float, float
// 		>(std::make_tuple(
// 			left, right
// 		))
// 	);
// 	}

// std::string Turn::repr()
// {
// 	std::stringstream ss;
// 	ss << "Turn, Left: " << left << "Right: " << right << ">\n";
// 	return ss.str();
// }
Move::Move(){

}

Move::Move(const Packet &p)
{
	std::tie(left, right) = deserialize<float, float>(p.data());
}

Packet Move::pack()
{
	return Packet(
		Move::id,
		serialize<
			float, float
		>(std::make_tuple(
			left, right)));
}

std::string Move::repr()
{
	std::stringstream ss;
	ss << "Move<left: " << left << " right: " << right << ">\n";
	return ss.str();
}

State::State(){

}

State::State(const Packet &p)
{
	std::tie(wireless) = deserialize<float>(p.data());
}

Packet State::pack()
{
	return Packet(
		State::id,
		serialize<
			float
		>(std::make_tuple(
			wireless)));
}

std::string State::repr()
{
	std::stringstream ss;
	ss << "Move<wireless: " << wireless << ">\n";
	return ss.str();
}


Sensor_Data::Sensor_Data()
{
}

Sensor_Data::Sensor_Data(const Packet &p)
{
	std::tie(time, potval, dist, wz, left, right, theta) =
		deserialize<int32_t, int32_t, int32_t, float, int32_t, int32_t, float>(p.data());
}

Packet Sensor_Data::pack()
{
	return Packet(
		Sensor_Data::id,
		serialize<
			int32_t, int32_t, int32_t, float, int32_t, int32_t, float>(std::make_tuple(
			time, potval, dist, wz, left, right, theta)));
}

std::string Sensor_Data::repr()
{
	std::stringstream ss;
	ss << "Sensor_Data<time: " << time << " potval: " << potval << " dist: " << dist << " wz: " << wz << " left: " << left << " right: " << right << " theta: " << theta << '\n';
	return ss.str();
}