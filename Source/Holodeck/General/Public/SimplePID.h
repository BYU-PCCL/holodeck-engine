// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class HOLODECK_API SimplePID
{
public:
	SimplePID();
	SimplePID(double p, double i, double d, double Tau);
	~SimplePID();

	/**
	* computePID
	*
	* Computes the control
	* @param desired the desired value
	* @param current the current value
	* @param dt the delta time
	* @return the PID
	*/
	float computePID(float desired, float current, float dt);

	float computePIDDirect(float x_c, float x, float x_dot, float dt);

	float computePIDDirect(float x_c, float x, float x_dot, float dt, bool isAngle);


	/**
	* setGains
	*
	* Used for late initialization or a redo
	*/
	void setGains(float p, float i, float d, float Tau);

	private:
	float integrator;
	float differentiator;
	float last_error;
	float last_state;

	float kp;
	float ki;
	float kd;
	float tau;

	float my_abs(float i);
};
