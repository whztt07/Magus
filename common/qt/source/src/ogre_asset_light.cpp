/****************************************************************************
**
** Copyright (C) 2014
**
** This file is generated by the Magus toolkit
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

// Include
#include <QMessageBox>
#include "ogre_asset_light.h"
#include "asset_propertywidget_xyz.h"
#include "asset_propertywidget_string.h"
#include "asset_propertywidget_select.h"
#include "asset_propertywidget_decimal.h"
#include "asset_propertywidget_color.h"

namespace Magus
{
    //****************************************************************************/
    QtOgreAssetLight::QtOgreAssetLight(const QString& iconDir, QWidget* parent) : QWidget(parent)
    {
        mOgreLight = 0;

        // Create QtAssetWidget
        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(0, 0, 0, 0);
        mAssetWidget = new QtAssetWidget(QString("Light"),
                                         iconDir + QString("light_off.png"),
                                         this);
        mAssetWidget->setFileNameIconCollapsed(iconDir + QString("collapse.png"));
        mAssetWidget->setFileNameIconExpanded(iconDir + QString("expand.png"));
        mAssetWidget->setHeaderTitleBold();
        connect(mAssetWidget, SIGNAL(valueChanged(QtProperty*)), this, SLOT(propertyValueChanged(QtProperty*)));

        // Create container - General
        QtContainerWidget* container = 0;
        container = mAssetWidget->createContainer(CONTAINER_GENERAL, QString("General"));
        container->setTitleIcon(iconDir + QString("cube_bold.png"));
        container->setTitleBold();
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_NAME_PROPERTY, QString("Name"), QtProperty::STRING, true);
        QStringList stringList;
        stringList << QString("Point light") << QString("Directional light") << QString("Spot light");
        QtSelectProperty* selectProperty = static_cast<QtSelectProperty*>
                                            (mAssetWidget->createProperty(CONTAINER_GENERAL,
                                            LIGHT_TYPE,
                                            QString("Type"),
                                            QtProperty::SELECT));
        selectProperty->addValues(stringList);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_ATTENUATION_CONSTANT, QString("Attenuation constant"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_ATTENUATION_LINEAR, QString("Attenuation linear"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_ATTENUATION_QUADRATIC, QString("Attenuation quadratic"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_ATTENUATION_RANGE, QString("Attenuation range"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_POWER_SCALE, QString("Power scale"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_SPOTLIGHT_FALLOFF, QString("Spotlight falloff"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_SPOTLIGHT_NEARCLIP_DIST, QString("Spotlight nearclip distance"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_SPOTLIGHT_INNERANGLE, QString("Spotlight inner angle"), QtProperty::DECIMAL);
        mAssetWidget->createProperty(CONTAINER_GENERAL, LIGHT_SPOTLIGHT_OUTERANGLE, QString("Spotlight outer angle"), QtProperty::DECIMAL);

        // Create container - Transform
        container = mAssetWidget->createContainer(CONTAINER_TRANSFORM, QString("Transform"));
        container->setTitleBold();
        container->setTitleIcon(iconDir + QString("axis_bold.png"));
        mAssetWidget->createProperty(CONTAINER_TRANSFORM, LIGHT_POSITION_PROPERTY, QString("Position"), QtProperty::XYZ);
        mAssetWidget->createProperty(CONTAINER_TRANSFORM, LIGHT_DIRECTION_PROPERTY, QString("Direction"), QtProperty::XYZ);

        // Create container - Color
        container = mAssetWidget->createContainer(CONTAINER_COLOR, QString("Color"));
        container->setTitleBold();
        container->setTitleIcon(iconDir + QString("color_bold.png"));
        QtColorProperty* colourProperty = static_cast<QtColorProperty*>
                (mAssetWidget->createProperty(CONTAINER_COLOR, LIGHT_COLOR_DIFFUSE, QString("Diffuse"), QtProperty::COLOR));
        colourProperty->setAlphaVisible(false);
        colourProperty = static_cast<QtColorProperty*>
                (mAssetWidget->createProperty(CONTAINER_COLOR, LIGHT_COLOR_SPECULAR, QString("Specular"), QtProperty::COLOR));
        colourProperty->setAlphaVisible(false);

         // Layout
        mainLayout->addWidget(mAssetWidget);
        setLayout(mainLayout);
    }

    //****************************************************************************/
    QtOgreAssetLight::~QtOgreAssetLight(void)
    {
    }

    //****************************************************************************/
    void QtOgreAssetLight::expandAll(void)
    {
        if (mAssetWidget)
            mAssetWidget->expandAllContainers();
    }

    //****************************************************************************/
    void QtOgreAssetLight::collapseAll(void)
    {
        if (mAssetWidget)
            mAssetWidget->collapseAllContainers();
    }

    //****************************************************************************/
    void QtOgreAssetLight::bindObject (Ogre::Light* light)
    {
        mOgreLight = light; // Allow null value lights
        if (!light)
            return;

        // Set the properties, based on the light settings
        // Name
        QtStringProperty* stringProperty = static_cast<QtStringProperty*>(mAssetWidget->getPropertyWidget(LIGHT_NAME_PROPERTY));
        const char* ch = light->getName().c_str();
        stringProperty->setString(QString(ch));

        // Type
        QtSelectProperty* selectProperty = static_cast<QtSelectProperty*>(mAssetWidget->getPropertyWidget(LIGHT_TYPE));
        if (light->getType() == Ogre::Light::LT_POINT)
            selectProperty->setCurentIndex(0);
        else if (light->getType() == Ogre::Light::LT_DIRECTIONAL)
            selectProperty->setCurentIndex(1);
        else
            selectProperty->setCurentIndex(2);

        // Attentuation constant
        QtDecimalProperty* decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_ATTENUATION_CONSTANT));
        decimalProperty->setValue(light->getAttenuationConstant());

        // Attentuation linear
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_ATTENUATION_LINEAR));
        decimalProperty->setValue(light->getAttenuationLinear());

        // Attentuation quadratic
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_ATTENUATION_QUADRATIC));
        decimalProperty->setValue(light->getAttenuationQuadric());

        // Attentuation range
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_ATTENUATION_RANGE));
        decimalProperty->setValue(light->getAttenuationRange());

        // Power scale
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_POWER_SCALE));
        decimalProperty->setValue(light->getPowerScale());

        // Spotlight falloff
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_SPOTLIGHT_FALLOFF));
        decimalProperty->setValue(light->getSpotlightFalloff());

        // Spotlight innerangle
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_SPOTLIGHT_INNERANGLE));
        decimalProperty->setValue(light->getSpotlightInnerAngle().valueDegrees());

        // Spotlight nearclip distance
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_SPOTLIGHT_NEARCLIP_DIST));
        decimalProperty->setValue(light->getSpotlightNearClipDistance());

        // Spotlight outer angle
        decimalProperty = static_cast<QtDecimalProperty*>(mAssetWidget->getPropertyWidget(LIGHT_SPOTLIGHT_OUTERANGLE));
        decimalProperty->setValue(light->getSpotlightOuterAngle().valueDegrees());

        // Position
        QtXYZProperty* xyzProperty = static_cast<QtXYZProperty*>(mAssetWidget->getPropertyWidget(LIGHT_POSITION_PROPERTY));
        Ogre::Vector3 pos = light->getPosition();
        xyzProperty->setXYZ(pos.x, pos.y, pos.z);

        // Direction
        xyzProperty = static_cast<QtXYZProperty*>(mAssetWidget->getPropertyWidget(LIGHT_DIRECTION_PROPERTY));
        Ogre::Vector3 dir = light->getDirection();
        xyzProperty->setXYZ(dir.x, dir.y, dir.z);

        // Diffuse
        QtColorProperty* colorProperty = static_cast<QtColorProperty*>(mAssetWidget->getPropertyWidget(LIGHT_COLOR_DIFFUSE));
        Ogre::ColourValue color = light->getDiffuseColour();
        colorProperty->setColor(255.0f * color.r, 255.0f * color.g, 255.0f * color.b, 255.0f * color.a);

        // Specular
        colorProperty = static_cast<QtColorProperty*>(mAssetWidget->getPropertyWidget(LIGHT_COLOR_SPECULAR));
        color = light->getSpecularColour();
        colorProperty->setColor(255.0f * color.r, 255.0f * color.g, 255.0f * color.b, 255.0f * color.a);
    }

    //****************************************************************************/
    void QtOgreAssetLight::propertyValueChanged(QtProperty* property)
    {
        if (!mOgreLight || !property)
            return;

        QtXYZProperty* xyzProperty = 0;
        QtSelectProperty* selectionProperty = 0;
        QtDecimalProperty* decimalProperty = 0;
        QtColorProperty* colorProperty = 0;
        switch (property->mPropertyId)
        {
            case LIGHT_TYPE:
            {
                selectionProperty = static_cast<QtSelectProperty*>(property);
                if (selectionProperty->getCurrentIndex() == 0)
                    mOgreLight->setType(Ogre::Light::LT_POINT);
                else if (selectionProperty->getCurrentIndex() == 1)
                    mOgreLight->setType(Ogre::Light::LT_DIRECTIONAL);
                else
                    mOgreLight->setType(Ogre::Light::LT_SPOTLIGHT);
            }
            break;

            case LIGHT_ATTENUATION_CONSTANT:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setAttenuation(mOgreLight->getAttenuationRange(),
                                           decimalProperty->getValue(),
                                           mOgreLight->getAttenuationLinear(),
                                           mOgreLight->getAttenuationQuadric());
            }
            break;

            case LIGHT_ATTENUATION_LINEAR:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setAttenuation(mOgreLight->getAttenuationRange(),
                                           mOgreLight->getAttenuationConstant(),
                                           decimalProperty->getValue(),
                                           mOgreLight->getAttenuationQuadric());
            }
            break;

            case LIGHT_ATTENUATION_QUADRATIC:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setAttenuation(mOgreLight->getAttenuationRange(),
                                           mOgreLight->getAttenuationConstant(),
                                           mOgreLight->getAttenuationLinear(),
                                           decimalProperty->getValue());
            }
            break;

            case LIGHT_ATTENUATION_RANGE:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setAttenuation(decimalProperty->getValue(),
                                           mOgreLight->getAttenuationConstant(),
                                           mOgreLight->getAttenuationLinear(),
                                           mOgreLight->getAttenuationQuadric());
            }
            break;

            case LIGHT_POWER_SCALE:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setPowerScale(decimalProperty->getValue());
            }
            break;

            case LIGHT_SPOTLIGHT_FALLOFF:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setSpotlightFalloff(decimalProperty->getValue());
            }
            break;

            case LIGHT_SPOTLIGHT_INNERANGLE:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setSpotlightInnerAngle(Ogre::Radian(Ogre::Degree(decimalProperty->getValue())));
            }
            break;

            case LIGHT_SPOTLIGHT_NEARCLIP_DIST:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setSpotlightNearClipDistance(decimalProperty->getValue());
            }
            break;

            case LIGHT_SPOTLIGHT_OUTERANGLE:
            {
                decimalProperty = static_cast<QtDecimalProperty*>(property);
                mOgreLight->setSpotlightOuterAngle(Ogre::Radian(Ogre::Degree(decimalProperty->getValue())));
            }
            break;

            case LIGHT_POSITION_PROPERTY:
            {
                xyzProperty = static_cast<QtXYZProperty*>(property);
                mOgreLight->setPosition(xyzProperty->getX(), xyzProperty->getY(), xyzProperty->getZ());
            }
            break;

            case LIGHT_DIRECTION_PROPERTY:
            {
                xyzProperty = static_cast<QtXYZProperty*>(property);
                mOgreLight->setDirection(xyzProperty->getX(), xyzProperty->getY(), xyzProperty->getZ());
            }
            break;

            case LIGHT_COLOR_DIFFUSE:
            {
                colorProperty = static_cast<QtColorProperty*>(property);
                mOgreLight->setDiffuseColour(colorProperty->getRed() / 255.0f,
                                             colorProperty->getGreen() / 255.0f,
                                             colorProperty->getBlue() / 255.0f);
            }
            break;

            case LIGHT_COLOR_SPECULAR:
            {
                colorProperty = static_cast<QtColorProperty*>(property);
                mOgreLight->setSpecularColour(colorProperty->getRed() / 255.0f,
                                              colorProperty->getGreen() / 255.0f,
                                              colorProperty->getBlue() / 255.0f);
            }
            break;
        }
    }
}
