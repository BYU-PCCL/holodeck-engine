// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "SimplePID.h"
#include "stdlib.h"

SimplePID::SimplePID()
{
	kp = 0.0f; // in context of our whole program, P is greater when mass is greater. These numbers are tuned to a certain drone. We may need to tune them again for UE4
	ki = 0.0f;
	kd = 0.0f;
	integrator = 0.0f;
	differentiator = 0.0f;
	last_error = 0.0f;
	last_state = 0.0f;
	tau = 0.0f; //Connor: What does tau represent?
}

SimplePID::SimplePID(double p, double i, double d, double Tau) :
	kp(p), ki(i), kd(d), tau(Tau)
{
	integrator = 0.0f;
	differentiator = 0.0f;
	last_error = 0.0f;
	last_state = 0.0f;
}

SimplePID::~SimplePID()
{
}

float SimplePID::computePID(float desired, float current, float dt)
{
	// Get the error
	float error = desired - current;

	// If dt is 0 or the error is massive, that's a probelm
	if (dt == 0.0 || my_abs(error) > 9999999)
	{
		last_error = error;
		last_state = current;
		return 0.0;
	}

	integrator += dt / 2 * (error + last_error);

	// Derivative
	if (dt > 0.0)
	{
		// Noise reduction (See "Small Unmanned Aircraft". Chapter 6. Slide 31/33)
		// d/dx w.r.t. error:: differentiator_ = (2*tau_ - dt)/(2*tau_ + dt)*differentiator_ + 2/(2*tau_ + dt)*(error - last_error_);
		differentiator = (2 * tau - dt) / (2 * tau + dt)*differentiator + 2 / (2 * tau + dt)*(current - last_state);
	}

	last_error = error;
	last_state = current;

	// Note the negative der. term.  This is because now the differentiator is in the feedback loop rather than the forward loop
	return kp*error + ki*integrator - kd*differentiator;
}

/**
* computePIDDirect
*
* Computes the PID directly?
* @param x_c desired
* @param x current
* @param x_dot the gradient (angular velocity?)
* @param dt the change in time
* @return force or torque
*/
float SimplePID::computePIDDirect(float x_c, float x, float x_dot, float dt)
{
	//x_c is the desired, x is the current
	float error = x_c - x;

	if (dt == 0 || my_abs(error) > 9999999)
	{
		last_error = error;
		last_state = x;
	}

	integrator += dt / 2 * (error + last_error);

	last_error = error;
	last_state = x;

	return kp*error + ki*integrator - kd*x_dot;
}

/**
* computePIDDirect
*
* Computes the PID directly?
* @param x_c desired
* @param x current
* @param x_dot the gradient (angular velocity?)
* @param dt the change in time
* @return force or torque
*/
float SimplePID::computePIDDirect(float x_c, float x, float x_dot, float dt, bool isAngle)
{
	//x_c is the desired, x is the current
	float error = x_c - x;

	if (isAngle) {
		if (error >= PI) 
			error = error - (2 * PI);
		else if (error <= -PI)
			error = error + (2 * PI);
	}

	if (dt == 0 || my_abs(error) > 9999999)
	{
		last_error = error;
		last_state = x;
	}

	integrator += dt / 2 * (error + last_error);

	last_error = error;
	last_state = x;

	return kp*error + ki*integrator - kd*x_dot;
}

void SimplePID::setGains(float p, float i, float d, float Tau)
{
	kp = p;
	ki = i;
	kd = d;

	tau = 0; // time for force to go from 0 to 63.2% of maximum desired force
	return;
}

float SimplePID::my_abs(float i)
{
	if (i < 0)
	return -i;
	else
	return i;
}