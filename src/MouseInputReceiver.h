#pragma once

class MouseInputReceiver
{
public:
	static int Start(int* dataBlock);
	static void Update();
	static void Stop();
};