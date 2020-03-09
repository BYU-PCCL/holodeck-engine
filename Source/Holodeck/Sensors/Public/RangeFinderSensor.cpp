// MIT License (c) 2019 BYU PCCL see LICENSE file

#include "Holodeck.h"
#include "RangeFinderSensor.h"
#include "Json.h"

URangeFinderSensor::URangeFinderSensor() {
	SensorName = "RangeFinderSensor";
}

// Allows sensor parameters to be set programmatically from client.
void URangeFinderSensor::ParseSensorParms(FString ParmsJson) {
	Super::ParseSensorParms(ParmsJson);

	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ParmsJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed)) {

		if (JsonParsed->HasTypedField<EJson::Number>("LazerCount")) {
			LazerCount = JsonParsed->GetIntegerField("LazerCount");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("LazerAngle")) {
			LazerAngle = JsonParsed->GetIntegerField("LazerAngle");
		}

		if (JsonParsed->HasTypedField<EJson::Number>("LazerMaxDistance")) {
			LazerMaxDistance = JsonParsed->GetIntegerField("LazerMaxDistance");
		}
	}
	else {
		UE_LOG(LogHolodeck, Fatal, TEXT("URangeFinderSensor::ParseSensorParms:: Unable to parse json."));
	}
}

void URangeFinderSensor::InitializeSensor() {
	Super::InitializeSensor();
	//You need to get the pointer to the object you are attached to. 
	Parent = this->GetAttachmentRootActor();
}

void URangeFinderSensor::TickSensorComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {

	float* FloatBuffer = static_cast<float*>(Buffer);

	for (int i = 0; i < LazerCount; i++) {

		FVector start = GetForwardVector();
		FVector right = GetRightVector();
		start = start.RotateAngleAxis(360 * i / LazerCount, GetUpVector());
		right = right.RotateAngleAxis(360 * i / LazerCount, GetUpVector());
		start.RotateAngleAxis(LazerAngle, right);

		FVector end = start * LazerMaxDistance;

		FCollisionQueryParams QueryParams = FCollisionQueryParams();
		QueryParams.AddIgnoredActor(Parent);

		FHitResult Hit = FHitResult();

		bool TraceResult = GetWorld()->LineTraceSingleByChannel(Hit, start, end, ECollisionChannel::ECC_Visibility, QueryParams);

		FloatBuffer[i] = Hit.Distance != NULL ? Hit.Distance : LazerMaxDistance;
	}
}
