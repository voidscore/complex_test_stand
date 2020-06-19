#include "navigational_equipment.h"

double NavigationalEquipment::getRange() const
{
    return range;
}

double NavigationalEquipment::getAzimuth() const
{
    return azimuth;
}

double NavigationalEquipment::getCourse() const
{
    return course;
}

double NavigationalEquipment::getGlide() const
{
    return glide;
}

bool NavigationalEquipment::getStatusRange() const
{
    return statusRange;
}

bool NavigationalEquipment::getStatusAzimuth() const
{
    return statusAzimuth;
}

bool NavigationalEquipment::getStatusCourse() const
{
    return statusCourse;
}

bool NavigationalEquipment::getStatusGlide() const
{
    return statusGlide;
}

NavigationalEquipment::NavigationalEquipment()
{
    range   = BAD_VALUE;
    azimuth = BAD_VALUE;
    course  = BAD_VALUE;
    glide   = BAD_VALUE;

    statusRange   = false;
    statusAzimuth = false;
    statusCourse  = false;
    statusGlide   = false;
}

NavigationalEquipment::~NavigationalEquipment()
{

}
