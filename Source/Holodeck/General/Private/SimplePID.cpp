// Fill out your copyright notice in the Description page of Project Settings.

#include "Holodeck.h"
#include "SimplePID.h"

SimplePID::SimplePID() {
	KP = 0.0f; // in context of our whole program, P is greater when mass is greater. 
	KI = 0.0f; // These numbers are tuned to a certain drone. We may need to tune them again for UE4.
	KD = 0.0f;
	Integrator = 0.0f;
	Differentiator = 0.0f;
	LastError = 0.0f;
	LastState = 0.0f;
	Tau = 0.0f; // Connor: What does tau represent?
}

SimplePID::SimplePID(double P, double I, double D, double Tau)
		: KP(P), KI(I), KD(D), Tau(Tau) {
	Integrator = 0.0f;
	Differentiator = 0.0f;
	LastError = 0.0f;
	LastState = 0.0f;
}

float SimplePID::ComputePID(float Desired, float Current, float Delta) {
	// Get the error
	float Error = Desired - Current;

	// If dt is 0 or the error is massive, that's a probelm
	if (Delta == 0.0 || MyAbs(Error) > 9999999) {
		LastError = Error;
		LastState = Current;
		return 0.0;
	}

	Integrator += Delta / 2.0 * (Error + LastError);

	// Derivative
	// Noise reduction (See "Small Unmanned Aircraft". Chapter 6. Slide 31/33)
	// d/dx w.r.t. error:: differentiator_ = (2*tau_ - dt)/(2*tau_ + dt)*differentiator_ + 2/(2*tau_ + dt)*(error - last_error_);
	if (Delta > 0.0)
		Differentiator = (2.0 * Tau - Delta) / (2.0 * Tau + Delta) * Differentiator + 2.0 / (2.0 * Tau + Delta) * (Current - LastState);

	LastError = Error;
	LastState = Current;

	// Note the negative der. term.  This is because now the differentiator is in the feedback loop rather than the forward loop
	return KP * Error + KI * Integrator - KD * Differentiator;
}

float SimplePID::ComputePIDDirect(float XC, float X, float XDot, float Delta) {
	//XC is the desired, X is the current
	float Error = XC - X;

	if (Delta == 0 || MyAbs(Error) > 9999999) {
		LastError = Error;
		LastState = X;
	}

	Integrator += Delta / 2.0 * (Error + LastError);

	LastError = Error;
	LastState = X;

	return KP * Error + KI * Integrator - KD * XDot;
}

float SimplePID::ComputePIDDirect(float XC, float X, float XDot, float Delta, bool bIsAngle) {
	// XC is the desired, X is the current
	float Error = XC - X;

	if (bIsAngle) {
		if (Error >= PI) 
			Error -= (2.0 * PI);
		else if (Error <= -PI)
			Error += (2 * PI);
	}

	if (Delta == 0 || MyAbs(Error) > 9999999) {
		LastError = Error;
		LastState = X;
	}

	Integrator += Delta / 2.0 * (Error + LastError);
	LastError = Error;
	LastState = X;

	return KP * Error + KI * Integrator - KD * XDot;
}

void SimplePID::SetGains(float P, float I, float D, float Tau) {
	KP = P;
	KI = I;
	KD = D;

	this->Tau = Tau; // time for force to go from 0 to 63.2% of maximum desired force
	return;
}

float SimplePID::MyAbs(float I){
	if (I < 0)
		return -I;
	else
		return I;
}
