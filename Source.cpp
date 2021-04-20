//CREDIT TO OneLoneCoder FOR THE ENGINE 
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <cmath>

// Override base class with your custom functionality
class Physics : public olc::PixelGameEngine
{
public:
	Physics()
	{
		// Name your application
		sAppName = "Physics";
	}

public:

	void BallPull(void);
	void DistanceFromOrigin(void);
	bool bPullBall(void);
	bool bPullSlider1(void);
	void ButtonPull1(void);
	void ButtonPull2(void);
	bool bPullSlider2(void);
	void UpdateValues(void);

	olc::vf2d vBall = { 320.0f, 240.0f };
	olc::vf2d vPreviousPos = { 320.0f, 240.0f };
	olc::vf2d vOrigin = { 320.0f, 240.0f };

	olc::vf2d vVelocity = { 0.0f, 0.0f };
	olc::vf2d vAcceleration = { 0.0f, 0.0f };

	olc::vf2d vButton1 = { 500, 410 };
	olc::vf2d vButton1Prev = { 500, 410 };
	olc::vf2d vButton1Dim = { 20, 20 };

	olc::vf2d vButton2 = { 100, 410 };
	olc::vf2d vButton2Prev = { 100, 410 };
	olc::vf2d vButton2Dim = { 20, 20 };

	float fMass = 1.0f;
	float fSpringConst = 1.0f;
	float fElapsedTimeAccumulated = 0;
	float fElapsedStore = 0;
	float fDistanceOnRelease = 0;
	float fOmega = sqrt(fSpringConst / fMass);
	float fVelocityMax = fDistanceOnRelease;
	float fAccelerationMax = fVelocityMax * fOmega;

	const float PI = 3.14f;

	bool bClickedOutside = false;

	int radius = 50;

	bool bBallClick = false;

	bool bButt1Click = false;
	bool bButt2Click = false;

	int but1min = 500;
	int but1max = 600;

	int but2min = 100;
	int but2max = 200;

	float fPrevOmega = 1.0f;

	uint32_t x = 320;
	uint32_t y = 40;
	uint32_t x1 = 50;
	uint32_t y1 = 50;
	uint32_t acc = 0;
	uint32_t vel = 0;

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{

		Clear(olc::DARK_BLUE);

		UpdateValues();

		BallPull();
		DistanceFromOrigin();
		ButtonPull1();
		ButtonPull2();

		FillCircle(vBall, radius, olc::WHITE);
		DrawString(320, 20, "Velocity: " + std::to_string(int(vVelocity.x)));
		DrawRect(320, 30, vel, 10);

		DrawString(320, 50, "Acceleration: " + std::to_string(int(vAcceleration.x)));
		DrawRect(320, 70, acc, 10);

		DrawLine(ScreenWidth()/2, 0, ScreenWidth()/2, ScreenHeight());
		DrawLine(0, ScreenHeight()/2, ScreenWidth(), ScreenHeight()/2);

		DrawString(550, 390, "Mass: " + std::to_string(int(fMass)));
		FillRect(vButton1, vButton1Dim);
		DrawLine(500, 420, 620, 420);

		DrawString(150, 390, "Spring Constant: " + std::to_string(int(fSpringConst)));
		FillRect(vButton2, vButton2Dim);
		DrawLine(100, 420, 200, 420);


		if (!bPullBall())
			fElapsedTimeAccumulated += fElapsedTime;




		return true;
	}
};

void Physics::BallPull(void)
{
	if (bPullBall())
	{
		if (GetMouse(0).bPressed)
		{
			vPreviousPos = { float(GetMouseX()), float(GetMouseY()) };
			bBallClick = true;
		}
		else if (bBallClick &&
			float(GetMouseX()) != vPreviousPos.x ||
			float(GetMouseY()) != vPreviousPos.y)
		{
			vBall.x = vBall.x + (float(GetMouseX()) - vPreviousPos.x);
			/*vBall = { vBall.x + (float(GetMouseX()) - vPreviousPos.x), vBall.y + (float(GetMouseY()) - vPreviousPos.y) };*/
			vPreviousPos = { float(GetMouseX()), float(GetMouseY()) };
		}
	}
}

void Physics::ButtonPull1(void)
{
	if (bPullSlider1() && vButton1.x <= but1max && vButton1.x >= but1min)
	{
		if (GetMouse(0).bPressed)
		{
			vButton1Prev = { float(GetMouseX()), float(GetMouseY()) };
			bButt1Click = true;
		}
		else if (bButt1Click &&
			float(GetMouseX()) != vButton1Prev.x ||
			float(GetMouseY()) != vButton1Prev.y)
		{
			vButton1.x = vButton1.x + (float(GetMouseX()) - vButton1Prev.x);
			vButton1Prev = { float(GetMouseX()), float(GetMouseY()) };
		}
	}
	else if (vButton1.x > but1max)
	{
		vButton1.x = but1max;
	}
	else if (vButton1.x < but1min)
	{
		vButton1.x = but1min;
	}
	else
		bButt1Click = false;
}


void Physics::ButtonPull2(void)
{
	if (bPullSlider2() && vButton2.x <= but2max && vButton2.x >= but2min)
	{
		if (GetMouse(0).bPressed)
		{
			vButton2Prev = { float(GetMouseX()), float(GetMouseY()) };
			bButt2Click = true;
		}
		else if (bButt2Click &&
			float(GetMouseX()) != vButton2Prev.x ||
			float(GetMouseY()) != vButton2Prev.y)
		{
			vButton2.x = vButton2.x + (float(GetMouseX()) - vButton2Prev.x);
			vButton2Prev = { float(GetMouseX()), float(GetMouseY()) };
		}
	}
	else if (vButton2.x > but2max)
	{
		vButton2.x = but2max;
	}
	else if (vButton2.x < but2min)
	{
		vButton2.x = but2min;
	}
	else
		bButt2Click = false;
}

void Physics::DistanceFromOrigin(void)
{
	if (GetMouse(0).bReleased && bBallClick)
	{
		fDistanceOnRelease = std::fabs(vOrigin.x - vBall.x);

		fVelocityMax = fDistanceOnRelease * fOmega;
		fAccelerationMax = fVelocityMax * fOmega;
		if (!GetMouse(0).bHeld || !GetMouse(0).bPressed) {
			fElapsedStore = fElapsedTimeAccumulated;
			fElapsedTimeAccumulated = (vOrigin.x - vBall.x > 0) ? PI / fOmega : 2 * PI / fOmega;
		}

		bBallClick = false;
	}

	if (!bPullBall())
	{
		vBall.x = vOrigin.x + fDistanceOnRelease * cos(fOmega * fElapsedTimeAccumulated);
		vVelocity.x = -fVelocityMax * sin(fOmega * fElapsedTimeAccumulated);
		vAcceleration.x = -fVelocityMax * cos(fOmega * fElapsedTimeAccumulated);

		acc = int(vAcceleration.x) / 5;
		vel = int(vVelocity.x) / 5;
	}
}

bool Physics::bPullBall()
{
	return GetMouse(0).bHeld &&
		std::fabs(vBall.x - float(GetMouseX())) <= radius &&
		std::fabs(vBall.y - float(GetMouseY())) <= radius;
}

bool Physics::bPullSlider1(void)
{
	return GetMouse(0).bHeld &&
		std::fabs(vButton1.x - float(GetMouseX())) <= vButton1Dim.x &&
		std::fabs(vButton1.y - float(GetMouseY())) <= vButton1Dim.y;
}

bool Physics::bPullSlider2(void)
{
	return GetMouse(0).bHeld &&
		std::fabs(vButton2.x - float(GetMouseX())) <= vButton2Dim.x &&
		std::fabs(vButton2.y - float(GetMouseY())) <= vButton2Dim.y;
}

void Physics::UpdateValues(void)
{
	fMass = vButton1.x - 499;
	if (fMass <= 0)
		fMass = 1;
	if (fMass >= 101)
		fMass = 100;
	fSpringConst = vButton2.x - 99;
	if (fSpringConst <= 0)
		fSpringConst = 1;
	if (fSpringConst >= 101)
		fSpringConst = 100;

	fPrevOmega = fOmega;

	fOmega = sqrt(fSpringConst / fMass);
	fVelocityMax = fDistanceOnRelease * fOmega;
	fAccelerationMax = fVelocityMax * fOmega;
}

int main()
{
	Physics demo;
	if (demo.Construct(640, 480, 1, 1))
		demo.Start();
	return 0;
}

