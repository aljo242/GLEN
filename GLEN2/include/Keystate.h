#pragma once


namespace GLEN
{
	// TODO rethink how the multi key scenarios in binary keycodes
enum class KeyState
{
	None = 0,
	Escape,
	W,
	A,
	S,
	D,
	WA,
	WS,
	WD,
	WAS,
	WAD,
	WSD,
	WASD,
};

} // namespace GLEN