//
// Created by josh on 5/9/17.
//

#include "holodeck.h"
#include "SubscriberInfo.h"

SubscriberInfo::SubscriberInfo() {
    this->agentName = "";
    this->name = "";
    this->startPos = 0;
    this->endPos = 0;
    this->length = 0;
}

SubscriberInfo::SubscriberInfo(const FString &agentName, const FString &name,
                               int startPos, int endPos, int length) {
    this->agentName = agentName;
    this->name = name;
    this->startPos = startPos;
    this->endPos = endPos;
    this->length = length;
}

void SubscriberInfo::setData(const FString &agentName, const FString &name,
                             int startPos, int endPos, int length) {
    this->agentName = agentName;
    this->name = name;
    this->startPos = startPos;
    this->endPos = endPos;
    this->length = length;
}

void SubscriberInfo::setAgentName(FString &agentName) {
    this->agentName = agentName;
}

void SubscriberInfo::setName(FString &name) {
    this->name = name;
}

void SubscriberInfo::setStartPos(int pos) {
    this->startPos = pos;
}

void SubscriberInfo::setEndPos(int pos) {
    this->endPos = pos;
}

void SubscriberInfo::setLength(int length) {
    this->length = length;
}

const FString &SubscriberInfo::getAgentName() const {
    return this->agentName;
}

const FString &SubscriberInfo::getName() const {
    return this->name;
}

int SubscriberInfo::getStartPos() const {
    return this->startPos;
}

int SubscriberInfo::getEndPos() const {
    return this->endPos;
}

int SubscriberInfo::getLength() const {
    return this->length;
}
