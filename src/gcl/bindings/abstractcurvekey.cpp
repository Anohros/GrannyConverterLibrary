#include "gcl/bindings/abstractcurvekey.h"

namespace GCL::Bindings {

AbstractCurveKey::AbstractCurveKey(GrannyCurve2 data)
    : m_data(data)
{
}

FbxTime AbstractCurveKey::getTime()
{
    return m_time;
}

void AbstractCurveKey::setTime(FbxDouble time)
{
    m_time.SetSecondDouble(time);
}

FbxDouble3 AbstractCurveKey::getValue()
{
    return m_value;
}

void AbstractCurveKey::setValue(FbxDouble3 value)
{
    m_value = value;
}

} // namespace GCL::Bindings
