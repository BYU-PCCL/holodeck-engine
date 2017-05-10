//
// Created by josh on 5/9/17.
//

#pragma once

#include "holodeck.h"
#include <string>

class SubscriberInfo {
public:
    SubscriberInfo();
    SubscriberInfo(const FString &agentName, const FString &name, int startPos, int endPos, int length);

    void setData(const FString &agentName, const FString &name, int startPos, int endPos, int length);

    void setAgentName(FString &agentName);
    void setName(FString &name);
    void setStartPos(int pos);
    void setEndPos(int pos);
    void setLength(int length);

    const FString& getAgentName() const;
    const FString& getName() const;
    int getStartPos() const;
    int getEndPos() const;
    int getLength() const;

private:
    FString agentName; // The name of the agent this belongs to
    FString name; // The identifier for this data
    int startPos;
    int endPos;
    int length;
};
