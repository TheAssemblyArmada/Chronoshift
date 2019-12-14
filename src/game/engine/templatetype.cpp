/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Class containing information about terrain templates.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "templatetype.h"
#include "templatedata.h"
#include "gamefile.h"
#include "coord.h"
#include "gamedebug.h"
#include "mixfile.h"
#include "iomap.h"
#include <algorithm>

#ifndef GAME_DLL
TFixedIHeapClass<TemplateTypeClass> g_TemplateTypes;
#endif

TemplateTypeClass::TemplateTypeClass(TemplateType type, int theater, const char *name, int uiname) :
    ObjectTypeClass(RTTI_TEMPLATETYPE, type, false, true, false, false, true, true, false, uiname, name),
    m_Type(type),
    m_Theater(theater),
    m_Width(0),
    m_Height(0)
{
    
}

TemplateTypeClass::TemplateTypeClass(const TemplateTypeClass &that) :
    ObjectTypeClass(that),
    m_Type(that.m_Type),
    m_Theater(that.m_Theater),
    m_Width(that.m_Width),
    m_Height(that.m_Height)
{
}

void *TemplateTypeClass::operator new(size_t size)
{
    DEBUG_ASSERT(size == sizeof(TemplateTypeClass));
    return g_TemplateTypes.Allocate();
}

void TemplateTypeClass::operator delete(void *ptr)
{
    DEBUG_ASSERT(ptr != nullptr);
    g_TemplateTypes.Free(ptr);
}

void TemplateTypeClass::Init_Heap()
{
    // Order of creation must match order of TemplateType enum.
    new TemplateTypeClass(TemplateClear);
    new TemplateTypeClass(TemplateWater);
    new TemplateTypeClass(TemplateWater2);

    new TemplateTypeClass(TemplateShore01);
    new TemplateTypeClass(TemplateShore02);
    new TemplateTypeClass(TemplateShore03);
    new TemplateTypeClass(TemplateShore04);
    new TemplateTypeClass(TemplateShore05);
    new TemplateTypeClass(TemplateShore06);
    new TemplateTypeClass(TemplateShore07);
    new TemplateTypeClass(TemplateShore08);
    new TemplateTypeClass(TemplateShore09);
    new TemplateTypeClass(TemplateShore10);
    new TemplateTypeClass(TemplateShore11);
    new TemplateTypeClass(TemplateShore12);
    new TemplateTypeClass(TemplateShore13);
    new TemplateTypeClass(TemplateShore14);
    new TemplateTypeClass(TemplateShore15);
    new TemplateTypeClass(TemplateShore16);
    new TemplateTypeClass(TemplateShore17);
    new TemplateTypeClass(TemplateShore18);
    new TemplateTypeClass(TemplateShore19);
    new TemplateTypeClass(TemplateShore20);
    new TemplateTypeClass(TemplateShore21);
    new TemplateTypeClass(TemplateShore22);
    new TemplateTypeClass(TemplateShore23);
    new TemplateTypeClass(TemplateShore24);
    new TemplateTypeClass(TemplateShore25);
    new TemplateTypeClass(TemplateShore26);
    new TemplateTypeClass(TemplateShore27);
    new TemplateTypeClass(TemplateShore28);
    new TemplateTypeClass(TemplateShore29);
    new TemplateTypeClass(TemplateShore30);
    new TemplateTypeClass(TemplateShore31);
    new TemplateTypeClass(TemplateShore32);
    new TemplateTypeClass(TemplateShore33);
    new TemplateTypeClass(TemplateShore34);
    new TemplateTypeClass(TemplateShore35);
    new TemplateTypeClass(TemplateShore36);
    new TemplateTypeClass(TemplateShore37);
    new TemplateTypeClass(TemplateShore38);
    new TemplateTypeClass(TemplateShore39);
    new TemplateTypeClass(TemplateShore40);
    new TemplateTypeClass(TemplateShore41);
    new TemplateTypeClass(TemplateShore42);
    new TemplateTypeClass(TemplateShore43);
    new TemplateTypeClass(TemplateShore44);
    new TemplateTypeClass(TemplateShore45);
    new TemplateTypeClass(TemplateShore46);
    new TemplateTypeClass(TemplateShore47);
    new TemplateTypeClass(TemplateShore48);
    new TemplateTypeClass(TemplateShore49);
    new TemplateTypeClass(TemplateShore50);
    new TemplateTypeClass(TemplateShore51);
    new TemplateTypeClass(TemplateShore52);
    new TemplateTypeClass(TemplateShore53);
    new TemplateTypeClass(TemplateShore54);
    new TemplateTypeClass(TemplateShore55);
    new TemplateTypeClass(TemplateShore56);

    new TemplateTypeClass(TemplateShoreCliff01);
    new TemplateTypeClass(TemplateShoreCliff02);
    new TemplateTypeClass(TemplateShoreCliff03);
    new TemplateTypeClass(TemplateShoreCliff04);
    new TemplateTypeClass(TemplateShoreCliff05);
    new TemplateTypeClass(TemplateShoreCliff06);
    new TemplateTypeClass(TemplateShoreCliff07);
    new TemplateTypeClass(TemplateShoreCliff08);
    new TemplateTypeClass(TemplateShoreCliff09);
    new TemplateTypeClass(TemplateShoreCliff10);
    new TemplateTypeClass(TemplateShoreCliff11);
    new TemplateTypeClass(TemplateShoreCliff12);
    new TemplateTypeClass(TemplateShoreCliff13);
    new TemplateTypeClass(TemplateShoreCliff14);
    new TemplateTypeClass(TemplateShoreCliff15);
    new TemplateTypeClass(TemplateShoreCliff16);
    new TemplateTypeClass(TemplateShoreCliff17);
    new TemplateTypeClass(TemplateShoreCliff18);
    new TemplateTypeClass(TemplateShoreCliff19);
    new TemplateTypeClass(TemplateShoreCliff20);
    new TemplateTypeClass(TemplateShoreCliff21);
    new TemplateTypeClass(TemplateShoreCliff22);
    new TemplateTypeClass(TemplateShoreCliff23);
    new TemplateTypeClass(TemplateShoreCliff24);
    new TemplateTypeClass(TemplateShoreCliff25);
    new TemplateTypeClass(TemplateShoreCliff26);
    new TemplateTypeClass(TemplateShoreCliff27);
    new TemplateTypeClass(TemplateShoreCliff28);
    new TemplateTypeClass(TemplateShoreCliff29);
    new TemplateTypeClass(TemplateShoreCliff30);
    new TemplateTypeClass(TemplateShoreCliff31);
    new TemplateTypeClass(TemplateShoreCliff32);
    new TemplateTypeClass(TemplateShoreCliff33);
    new TemplateTypeClass(TemplateShoreCliff34);
    new TemplateTypeClass(TemplateShoreCliff35);
    new TemplateTypeClass(TemplateShoreCliff36);
    new TemplateTypeClass(TemplateShoreCliff37);
    new TemplateTypeClass(TemplateShoreCliff38);

    new TemplateTypeClass(TemplateBoulder1);
    new TemplateTypeClass(TemplateBoulder2);
    new TemplateTypeClass(TemplateBoulder3);
    new TemplateTypeClass(TemplateBoulder4);
    new TemplateTypeClass(TemplateBoulder5);
    new TemplateTypeClass(TemplateBoulder6);

    new TemplateTypeClass(TemplatePatch01);
    new TemplateTypeClass(TemplatePatch02);
    new TemplateTypeClass(TemplatePatch03);
    new TemplateTypeClass(TemplatePatch04);
    new TemplateTypeClass(TemplatePatch07);
    new TemplateTypeClass(TemplatePatch08);
    new TemplateTypeClass(TemplatePatch13);
    new TemplateTypeClass(TemplatePatch14);
    new TemplateTypeClass(TemplatePatch15);

    new TemplateTypeClass(TemplateRiver01);
    new TemplateTypeClass(TemplateRiver02);
    new TemplateTypeClass(TemplateRiver03);
    new TemplateTypeClass(TemplateRiver04);
    new TemplateTypeClass(TemplateRiver05);
    new TemplateTypeClass(TemplateRiver06);
    new TemplateTypeClass(TemplateRiver07);
    new TemplateTypeClass(TemplateRiver08);
    new TemplateTypeClass(TemplateRiver09);
    new TemplateTypeClass(TemplateRiver10);
    new TemplateTypeClass(TemplateRiver11);
    new TemplateTypeClass(TemplateRiver12);
    new TemplateTypeClass(TemplateRiver13);

    new TemplateTypeClass(TemplateFalls1);
    new TemplateTypeClass(TemplateFalls1a);
    new TemplateTypeClass(TemplateFalls2);
    new TemplateTypeClass(TemplateFalls2a);

    new TemplateTypeClass(TemplateFord1);
    new TemplateTypeClass(TemplateFord2);

    new TemplateTypeClass(TemplateBridge1);
    new TemplateTypeClass(TemplateBridge1d);
    new TemplateTypeClass(TemplateBridge2);
    new TemplateTypeClass(TemplateBridge2d);

    new TemplateTypeClass(TemplateSlope01);
    new TemplateTypeClass(TemplateSlope02);
    new TemplateTypeClass(TemplateSlope03);
    new TemplateTypeClass(TemplateSlope04);
    new TemplateTypeClass(TemplateSlope05);
    new TemplateTypeClass(TemplateSlope06);
    new TemplateTypeClass(TemplateSlope07);
    new TemplateTypeClass(TemplateSlope08);
    new TemplateTypeClass(TemplateSlope09);
    new TemplateTypeClass(TemplateSlope10);
    new TemplateTypeClass(TemplateSlope11);
    new TemplateTypeClass(TemplateSlope12);
    new TemplateTypeClass(TemplateSlope13);
    new TemplateTypeClass(TemplateSlope14);
    new TemplateTypeClass(TemplateSlope15);
    new TemplateTypeClass(TemplateSlope16);
    new TemplateTypeClass(TemplateSlope17);
    new TemplateTypeClass(TemplateSlope18);
    new TemplateTypeClass(TemplateSlope19);
    new TemplateTypeClass(TemplateSlope20);
    new TemplateTypeClass(TemplateSlope21);
    new TemplateTypeClass(TemplateSlope22);
    new TemplateTypeClass(TemplateSlope23);
    new TemplateTypeClass(TemplateSlope24);
    new TemplateTypeClass(TemplateSlope25);
    new TemplateTypeClass(TemplateSlope26);
    new TemplateTypeClass(TemplateSlope27);
    new TemplateTypeClass(TemplateSlope28);
    new TemplateTypeClass(TemplateSlope29);
    new TemplateTypeClass(TemplateSlope30);
    new TemplateTypeClass(TemplateSlope31);
    new TemplateTypeClass(TemplateSlope32);
    new TemplateTypeClass(TemplateSlope33);
    new TemplateTypeClass(TemplateSlope34);
    new TemplateTypeClass(TemplateSlope35);
    new TemplateTypeClass(TemplateSlope36);
    new TemplateTypeClass(TemplateSlope37);
    new TemplateTypeClass(TemplateSlope38);

    new TemplateTypeClass(TemplateRoad01);
    new TemplateTypeClass(TemplateRoad02);
    new TemplateTypeClass(TemplateRoad03);
    new TemplateTypeClass(TemplateRoad04);
    new TemplateTypeClass(TemplateRoad05);
    new TemplateTypeClass(TemplateRoad06);
    new TemplateTypeClass(TemplateRoad07);
    new TemplateTypeClass(TemplateRoad08);
    new TemplateTypeClass(TemplateRoad09);
    new TemplateTypeClass(TemplateRoad10);
    new TemplateTypeClass(TemplateRoad11);
    new TemplateTypeClass(TemplateRoad12);
    new TemplateTypeClass(TemplateRoad13);
    new TemplateTypeClass(TemplateRoad14);
    new TemplateTypeClass(TemplateRoad15);
    new TemplateTypeClass(TemplateRoad16);
    new TemplateTypeClass(TemplateRoad17);
    new TemplateTypeClass(TemplateRoad18);
    new TemplateTypeClass(TemplateRoad19);
    new TemplateTypeClass(TemplateRoad20);
    new TemplateTypeClass(TemplateRoad21);
    new TemplateTypeClass(TemplateRoad22);
    new TemplateTypeClass(TemplateRoad23);
    new TemplateTypeClass(TemplateRoad24);
    new TemplateTypeClass(TemplateRoad25);
    new TemplateTypeClass(TemplateRoad26);
    new TemplateTypeClass(TemplateRoad27);
    new TemplateTypeClass(TemplateRoad28);
    new TemplateTypeClass(TemplateRoad29);
    new TemplateTypeClass(TemplateRoad30);
    new TemplateTypeClass(TemplateRoad31);
    new TemplateTypeClass(TemplateRoad32);
    new TemplateTypeClass(TemplateRoad33);
    new TemplateTypeClass(TemplateRoad34);
    new TemplateTypeClass(TemplateRoad35);
    new TemplateTypeClass(TemplateRoad36);
    new TemplateTypeClass(TemplateRoad37);
    new TemplateTypeClass(TemplateRoad38);
    new TemplateTypeClass(TemplateRoad39);
    new TemplateTypeClass(TemplateRoad40);
    new TemplateTypeClass(TemplateRoad41);
    new TemplateTypeClass(TemplateRoad42);
    new TemplateTypeClass(TemplateRoad43);

    new TemplateTypeClass(TemplateRough01);
    new TemplateTypeClass(TemplateRough02);
    new TemplateTypeClass(TemplateRough03);
    new TemplateTypeClass(TemplateRough04);
    new TemplateTypeClass(TemplateRough05);
    new TemplateTypeClass(TemplateRough06);
    new TemplateTypeClass(TemplateRough07);
    new TemplateTypeClass(TemplateRough08);
    new TemplateTypeClass(TemplateRough09);
    new TemplateTypeClass(TemplateRough10);
    new TemplateTypeClass(TemplateRough11);

    new TemplateTypeClass(TemplateRoad44);
    new TemplateTypeClass(TemplateRoad45);

    new TemplateTypeClass(TemplateRiver14);
    new TemplateTypeClass(TemplateRiver15);

    new TemplateTypeClass(TemplateRiverCliff01);
    new TemplateTypeClass(TemplateRiverCliff02);
    new TemplateTypeClass(TemplateRiverCliff03);
    new TemplateTypeClass(TemplateRiverCliff04);

    new TemplateTypeClass(TemplateBridge1a);
    new TemplateTypeClass(TemplateBridge1b);
    new TemplateTypeClass(TemplateBridge1c);

    new TemplateTypeClass(TemplateBridge2a);
    new TemplateTypeClass(TemplateBridge2b);
    new TemplateTypeClass(TemplateBridge2c);

    new TemplateTypeClass(TemplateBridge3a);
    new TemplateTypeClass(TemplateBridge3b);
    new TemplateTypeClass(TemplateBridge3c);
    new TemplateTypeClass(TemplateBridge3d);
    new TemplateTypeClass(TemplateBridge3e);
    new TemplateTypeClass(TemplateBridge3f);

    new TemplateTypeClass(TemplateF01);
    new TemplateTypeClass(TemplateF02);
    new TemplateTypeClass(TemplateF03);
    new TemplateTypeClass(TemplateF04);
    new TemplateTypeClass(TemplateF05);
    new TemplateTypeClass(TemplateF06);

    new TemplateTypeClass(TemplateArrow01);
    new TemplateTypeClass(TemplateArrow02);
    new TemplateTypeClass(TemplateArrow03);
    new TemplateTypeClass(TemplateArrow04);
    new TemplateTypeClass(TemplateArrow05);
    new TemplateTypeClass(TemplateArrow06);
    new TemplateTypeClass(TemplateArrow07);
    new TemplateTypeClass(TemplateArrow08);
    new TemplateTypeClass(TemplateArrow09);
    new TemplateTypeClass(TemplateArrow10);
    new TemplateTypeClass(TemplateArrow11);
    new TemplateTypeClass(TemplateArrow12);
    new TemplateTypeClass(TemplateArrow13);
    new TemplateTypeClass(TemplateArrow14);
    new TemplateTypeClass(TemplateArrow15);

    new TemplateTypeClass(TemplateFLOR0001);
    new TemplateTypeClass(TemplateFLOR0002);
    new TemplateTypeClass(TemplateFLOR0003);
    new TemplateTypeClass(TemplateFLOR0004);
    new TemplateTypeClass(TemplateFLOR0005);
    new TemplateTypeClass(TemplateFLOR0006);
    new TemplateTypeClass(TemplateFLOR0007);

    new TemplateTypeClass(TemplateGFLR0001);
    new TemplateTypeClass(TemplateGFLR0002);
    new TemplateTypeClass(TemplateGFLR0003);
    new TemplateTypeClass(TemplateGFLR0004);
    new TemplateTypeClass(TemplateGFLR0005);
    new TemplateTypeClass(TemplateGSTR0001);
    new TemplateTypeClass(TemplateGSTR0002);
    new TemplateTypeClass(TemplateGSTR0003);
    new TemplateTypeClass(TemplateGSTR0004);
    new TemplateTypeClass(TemplateGSTR0005);
    new TemplateTypeClass(TemplateGSTR0006);
    new TemplateTypeClass(TemplateGSTR0007);
    new TemplateTypeClass(TemplateGSTR0008);
    new TemplateTypeClass(TemplateGSTR0009);
    new TemplateTypeClass(TemplateGSTR0010);
    new TemplateTypeClass(TemplateGSTR0011);

    new TemplateTypeClass(TemplateLWAL0001);
    new TemplateTypeClass(TemplateLWAL0002);
    new TemplateTypeClass(TemplateLWAL0003);
    new TemplateTypeClass(TemplateLWAL0004);
    new TemplateTypeClass(TemplateLWAL0005);
    new TemplateTypeClass(TemplateLWAL0006);
    new TemplateTypeClass(TemplateLWAL0007);
    new TemplateTypeClass(TemplateLWAL0008);
    new TemplateTypeClass(TemplateLWAL0009);
    new TemplateTypeClass(TemplateLWAL0010);
    new TemplateTypeClass(TemplateLWAL0011);
    new TemplateTypeClass(TemplateLWAL0012);
    new TemplateTypeClass(TemplateLWAL0013);
    new TemplateTypeClass(TemplateLWAL0014);
    new TemplateTypeClass(TemplateLWAL0015);
    new TemplateTypeClass(TemplateLWAL0016);
    new TemplateTypeClass(TemplateLWAL0017);
    new TemplateTypeClass(TemplateLWAL0018);
    new TemplateTypeClass(TemplateLWAL0019);
    new TemplateTypeClass(TemplateLWAL0020);
    new TemplateTypeClass(TemplateLWAL0021);
    new TemplateTypeClass(TemplateLWAL0022);
    new TemplateTypeClass(TemplateLWAL0023);
    new TemplateTypeClass(TemplateLWAL0024);
    new TemplateTypeClass(TemplateLWAL0025);
    new TemplateTypeClass(TemplateLWAL0026);
    new TemplateTypeClass(TemplateLWAL0027);

    new TemplateTypeClass(TemplateStripe01);
    new TemplateTypeClass(TemplateStripe02);
    new TemplateTypeClass(TemplateStripe03);
    new TemplateTypeClass(TemplateStripe04);
    new TemplateTypeClass(TemplateStripe05);
    new TemplateTypeClass(TemplateStripe06);
    new TemplateTypeClass(TemplateStripe07);
    new TemplateTypeClass(TemplateStripe08);
    new TemplateTypeClass(TemplateStripe09);
    new TemplateTypeClass(TemplateStripe10);
    new TemplateTypeClass(TemplateStripe11);

    new TemplateTypeClass(TemplateWall01);
    new TemplateTypeClass(TemplateWall02);
    new TemplateTypeClass(TemplateWall03);
    new TemplateTypeClass(TemplateWall04);
    new TemplateTypeClass(TemplateWall05);
    new TemplateTypeClass(TemplateWall06);
    new TemplateTypeClass(TemplateWall07);
    new TemplateTypeClass(TemplateWall08);
    new TemplateTypeClass(TemplateWall09);
    new TemplateTypeClass(TemplateWall10);
    new TemplateTypeClass(TemplateWall11);
    new TemplateTypeClass(TemplateWall12);
    new TemplateTypeClass(TemplateWall13);
    new TemplateTypeClass(TemplateWall14);
    new TemplateTypeClass(TemplateWall15);
    new TemplateTypeClass(TemplateWall16);
    new TemplateTypeClass(TemplateWall17);
    new TemplateTypeClass(TemplateWall18);
    new TemplateTypeClass(TemplateWall19);
    new TemplateTypeClass(TemplateWall20);
    new TemplateTypeClass(TemplateWall21);
    new TemplateTypeClass(TemplateWall22);
    new TemplateTypeClass(TemplateWall23);
    new TemplateTypeClass(TemplateWall24);
    new TemplateTypeClass(TemplateWall25);
    new TemplateTypeClass(TemplateWall26);
    new TemplateTypeClass(TemplateWall27);
    new TemplateTypeClass(TemplateWall28);
    new TemplateTypeClass(TemplateWall29);
    new TemplateTypeClass(TemplateWall30);
    new TemplateTypeClass(TemplateWall31);
    new TemplateTypeClass(TemplateWall32);
    new TemplateTypeClass(TemplateWall33);
    new TemplateTypeClass(TemplateWall34);
    new TemplateTypeClass(TemplateWall35);
    new TemplateTypeClass(TemplateWall36);
    new TemplateTypeClass(TemplateWall37);
    new TemplateTypeClass(TemplateWall38);
    new TemplateTypeClass(TemplateWall39);
    new TemplateTypeClass(TemplateWall40);
    new TemplateTypeClass(TemplateWall41);
    new TemplateTypeClass(TemplateWall42);
    new TemplateTypeClass(TemplateWall43);
    new TemplateTypeClass(TemplateWall44);
    new TemplateTypeClass(TemplateWall45);
    new TemplateTypeClass(TemplateWall46);
    new TemplateTypeClass(TemplateWall47);
    new TemplateTypeClass(TemplateWall48);
    new TemplateTypeClass(TemplateWall49);

    new TemplateTypeClass(TemplateBridge1h);
    new TemplateTypeClass(TemplateBridge2h);
    new TemplateTypeClass(TemplateBridge1ax);
    new TemplateTypeClass(TemplateBridge2ax);
    new TemplateTypeClass(TemplateBridge1x);
    new TemplateTypeClass(TemplateBridge2x);

    new TemplateTypeClass(TemplateXtra01);
    new TemplateTypeClass(TemplateXtra02);
    new TemplateTypeClass(TemplateXtra03);
    new TemplateTypeClass(TemplateXtra04);
    new TemplateTypeClass(TemplateXtra05);
    new TemplateTypeClass(TemplateXtra06);
    new TemplateTypeClass(TemplateXtra07);
    new TemplateTypeClass(TemplateXtra08);
    new TemplateTypeClass(TemplateXtra09);
    new TemplateTypeClass(TemplateXtra10);
    new TemplateTypeClass(TemplateXtra11);
    new TemplateTypeClass(TemplateXtra12);
    new TemplateTypeClass(TemplateXtra13);
    new TemplateTypeClass(TemplateXtra14);
    new TemplateTypeClass(TemplateXtra15);
    new TemplateTypeClass(TemplateXtra16);

    new TemplateTypeClass(TemplateAntHill);

}

void TemplateTypeClass::Init(TheaterType theater)
{
    // TODO Needs IconSet functions and TheaterTypeClass
#ifdef GAME_DLL
    void(*func)(TheaterType) = reinterpret_cast<void(*)(TheaterType)>(0x0049EAB0);
    func(theater);
#elif 0
    char buffer[256];

    for (TemplateType i = TEMPLATE_FIRST; i < TEMPLATE_COUNT; ++i) {
        TemplateTypeClass &tem = As_Reference(i);

        if (((1 << theater) & tem.m_Theater) != 0) {
            const char *name = tem.m_ImageName[0] != '\0' ? tem.m_ImageName : tem.m_Name;

            sprintf(buffer, "%s.%s", name, TheaterTypeClass::As_Reference(theater).FileExt);
            tem.ImageData = GameFileClass::Retrieve(buffer);

            IconSetClass::Register_Icon_Set(tem.ImageData, true);

            tem.Width = 0;
            tem.Height = 0;

            if (tem.ImageData != nullptr) {
                tem.Width = std::max(int16_t(0), IconSetClass::Get_Icon_Set_Width(static_cast<IconControlType *>(tem.ImageData)));
                tem.Height = std::max(int16_t(0), IconSetClass::Get_Icon_Set_Height(static_cast<IconControlType *>(tem.ImageData)));

            }

        }

    }
#else

#endif
}

/**
 * @brief 
 *
 * @address 0x004B1B80 (beta)
 */
void TemplateTypeClass::Prep_For_Add()
{
    for (TemplateType i = TEMPLATE_FIRST; i < TEMPLATE_COUNT; ++i) {
        TemplateTypeClass *ttptr = As_Pointer(i);
        if (ttptr != nullptr) {
            if (ttptr->m_ImageData != nullptr) {
                g_Map.Add_To_List(ttptr);
            }
        }
    }
}

BOOL TemplateTypeClass::Create_And_Place(int16_t cellnum, HousesType house) const
{
    // TODO requires TemplateClass
#ifdef GAME_DLL
    BOOL(*func)(const TemplateTypeClass*, int16_t, HousesType) = reinterpret_cast<BOOL(*)(const TemplateTypeClass*, int16_t, HousesType)>(0x0049EB84);
    return func(this, cellnum, house);
#elif 0
    DEBUG_ASSERT(this != nullptr);

    TemplateClass *tptr = new TemplateClass(m_Type, cellnum);
    DEBUG_ASSERT(tptr != nullptr);

    return tptr != nullptr;
#else
    return false;
#endif
}

ObjectClass *TemplateTypeClass::Create_One_Of(HouseClass *house) const
{
    // TODO requires TemplateClass
#ifdef GAME_DLL
    ObjectClass *(*func)(const TemplateTypeClass*, HouseClass*) = reinterpret_cast<ObjectClass *(*)(const TemplateTypeClass*, HouseClass*)>(0x0049EBBC);
    return func(this, house);
#elif 0
    DEBUG_ASSERT(this != nullptr);
    //DEBUG_ASSERT(house != nullptr);

    TemplateClass *tptr = new TemplateClass(m_Type);
    DEBUG_ASSERT(tptr != nullptr);

    return tptr;
#else
    return nullptr;
#endif
}

LandType TemplateTypeClass::Land_Type(int sub_icon) const
{
    // TODO requires IconControlType
#ifdef GAME_DLL
    LandType(*func)(const TemplateTypeClass*, int) = reinterpret_cast<LandType(*)(const TemplateTypeClass*, int)>(0x0049E98C);
    return func(this, sub_icon);
#elif 0
    // I think it works like this, the enum entries are the index into _land
    // which matches the Template _LandType to th actual LandType type.
    enum TemplateLandType
    {
        TLAND_NONE = LAND_NONE,
        TLAND_FIRST = 0,
        TLAND_0 = 0,
        TLAND_1 = 1,        //used for the ant hill.
        TLAND_2 = 2,
        TLAND_3 = 3,        //used for roads, water, shores...
        TLAND_4 = 4,        //used for what i think are interior tiles.
        TLAND_5 = 5,
        TLAND_6 = 6,
        TLAND_7 = 7,        //used for clear.
        TLAND_8 = 8,
        TLAND_9 = 9,
        TLAND_10 = 10,
        TLAND_11 = 11,
        TLAND_12 = 12,
        TLAND_13 = 13,
        TLAND_14 = 14,
        TLAND_15 = 15,
        TLAND_COUNT = 16
    };

    static LandType _land[TLAND_COUNT] = {
        LAND_CLEAR, LAND_CLEAR, LAND_CLEAR, LAND_CLEAR,
        LAND_CLEAR, LAND_CLEAR, LAND_BEACH, LAND_CLEAR,
        LAND_ROCK,  LAND_ROAD,  LAND_WATER, LAND_RIVER,
        LAND_CLEAR, LAND_CLEAR, LAND_ROUGH, LAND_CLEAR
    };

    IconControlType *icon = (IconControlType *)ImageData;
    char *cmaps = nullptr;

    //accesses the raw image data of the template and get the _landtype.
    //Realistically, the assignment probably won't fail as its just a spot of
    //pointer arithmetic on a pointer we already checked wasn't NULL
    if (ImageData != nullptr && (cmaps = (char *)icon + icon->ColorMap) != nullptr) {
        return _land[cmaps[sub_icon % icon->MapWidth * icon->MapHeight]];
    }

    return LAND_NONE;
#else
    return LAND_NONE;
#endif
}

TemplateType TemplateTypeClass::From_Name(const char *name)
{
    DEBUG_ASSERT(name != nullptr);

    if (strcasecmp(name, "<none>") == 0 || strcasecmp(name, "none") == 0) {
        return TEMPLATE_NONE;
    }

    if (name != nullptr) {
        for (TemplateType type = TEMPLATE_FIRST; type < TEMPLATE_COUNT; ++type) {

            if (strcasecmp(name, Name_From(type)) == 0) {
                return type;
            }

        }

    }

    return TEMPLATE_NONE;
}

const char *TemplateTypeClass::Name_From(TemplateType type)
{
    return type != TEMPLATE_NONE && type < TEMPLATE_COUNT ? As_Reference(type).m_Name : "<none>";
}


const int16_t *TemplateTypeClass::Occupy_List(BOOL a1) const
{
    // TODO requires IconControlType
#ifdef GAME_DLL
    const int16_t *(*func)(const TemplateTypeClass*, BOOL) = reinterpret_cast<const int16_t *(*)(const TemplateTypeClass*, BOOL)>(0x0049EA2C);
    return func(this, a1);
#else
    return nullptr;
#endif
}

TemplateTypeClass *TemplateTypeClass::As_Pointer(TemplateType type)
{
    return (type < TEMPLATE_COUNT) && (type != TEMPLATE_NONE) ? &g_TemplateTypes[type] : nullptr;
}

TemplateTypeClass &TemplateTypeClass::As_Reference(TemplateType type)
{
    DEBUG_ASSERT(type != TEMPLATE_NONE);
    DEBUG_ASSERT(type < TEMPLATE_COUNT);

    return g_TemplateTypes[type];
}

coord_t TemplateTypeClass::Coord_Fixup(coord_t coord) const
{
    return Coord_Top_Left(coord);
}
