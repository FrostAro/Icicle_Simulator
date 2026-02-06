#include "Buff.h"
#include <string>

int Buff::ID = 0;
std::string Buff::name = "Buff";

Buff::Buff(Person *p)
    : buffID(Buff::getID()),
      p(p)
      
{
    ID++;
}

void Buff::addStack(const double n)
{
    if (this->stack < this->maxStack)
    {
        this->lastStack = stack;
        this->stack += n;
    }
}

bool Buff::reduceDuration(int deltaTime)
{
    if (this->duration >= 0)
    {
        this->duration -= deltaTime;
        return true;
    }
    return false;
}

bool Buff::operator==(const Buff &) const { return ID == this->getBuffID(); }

int Buff::getID() { return Buff::ID; }

void Buff::resetDuration() { this->duration = this->maxDuration; }

std::string Buff::getBuffName() const { return this->name; }

int Buff::getBuffID() const { return this->buffID; }
double Buff::getStack() const { return this->stack; }
double Buff::getMaxStack() const { return this->maxStack; }
double Buff::getLastStack() const { return this->lastStack; }
double Buff::getNumber() const { return this->number; }
double Buff::getDuration() const { return this->duration; }
double Buff::getMaxDuration() const { return this->maxDuration; }
bool Buff::getIsStackable() const { return this->isStackable; }
bool Buff::getAllowDuplicates() const { return this->allowDuplicates; }
bool Buff::getIsInherent() const { return this->isInherent; }
void Buff::resetID() { Buff::ID = 0; }
