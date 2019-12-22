/**
 * @file
 *
 * @author OmniBlade
 *
 * @brief Chronal vortex class for handling the special vortex effect.
 *
 * @copyright Chronoshift is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "vortex.h"
#include "building.h"
#include "coord.h"
#include "gamefile.h"
#include "gametypes.h"
#include "globals.h"
#include "iomap.h"
#include "mixfile.h"
#include "palette.h"
#include "pipe.h"
#include "rgb.h"
#include "rules.h"
#include "scenario.h"
#include "straw.h"
#include "target.h"
#include "theater.h"
#include "tileset.h"
#include "voc.h"
#include <algorithm>

#ifndef GAME_DLL
ChronalVortexClass g_ChronalVortex;
#endif

/**
 * 0x0058E07C
 */
ChronalVortexClass::ChronalVortexClass() :
    m_IsActive(false),
    m_ToHide(false),
    m_IsHidden(false),
    m_ToDeactivate(false),
    m_Theater(THEATER_NONE),
    m_VortexRange(10),
    m_VortexSpeed(10),
    m_VortexDamage(200),
    m_VortexGBuffer(nullptr)
{
}

/**
 * 0x0058E0C4
 */
ChronalVortexClass::~ChronalVortexClass()
{
    if (m_VortexGBuffer != nullptr) {
        delete m_VortexGBuffer;
    }

    m_VortexGBuffer = nullptr;
}

/**
 * If not already active, causes the vortex to appear at a specific location.
 *
 * 0x0058E0F4
 */
void ChronalVortexClass::Appear(coord_t coord)
{
    if (!m_IsActive) {
        m_FrameIncrement = 1;
        m_Frame = 0;
        m_AIState = VORTEX_STATE_0;
        m_SkipStateTick = false;
        m_AttackState = 0;
        m_SpeedX = 0;
        m_SpeedY = 0;

        // This conversion from lepton to pixel and back again, rounds the coord
        // down to the nearest pixel exactly.
        m_CurrentPos = Round_Coord_To_Pixel(coord);
        m_IsActive = true;
        m_ToHide = false;
        m_IsHidden = false;
        m_ToDeactivate = false;
        m_TargetSetFrame = g_GameFrame;
        m_RandomSpeedX = g_Scen.Get_Random_Value(-m_VortexSpeed, m_VortexSpeed);
        m_RandomSpeedY = g_Scen.Get_Random_Value(-m_VortexSpeed, m_VortexSpeed);
    }
}

/**
 * Depending on internal state will make the vortex either deactivate or flag to go invisible.
 *
 * 0x0058E240
 */
void ChronalVortexClass::Disappear()
{
    if (m_ToDeactivate) {
        m_IsActive = false;
    } else {
        m_ToHide = true;
    }
}

/**
 * Makes the vortex invisible if flagged to hide.
 *
 * 0x0058E270
 */
void ChronalVortexClass::Hide()
{
    if (m_ToHide) {
        m_IsHidden = true;
    }
}

/**
 * Makes the vortex visible if not flagged to hide.
 *
 * 0x0058E290
 */
void ChronalVortexClass::Show()
{
    if (!m_ToHide) {
        if (m_ToDeactivate) {
            m_Frame = 0;
            m_AIState = VORTEX_STATE_0;
            m_SpeedX = 0;
            m_SpeedY = 0;
            m_IsHidden = false;
            m_ToDeactivate = false;
        } else {
            m_IsHidden = false;

            if (m_AIState == VORTEX_STATE_2) {
                m_Frame -= 16;
                m_AIState = VORTEX_STATE_0;
            }
        }
    }
}

/**
 * Deactivates the vortex if active.
 *
 * 0x0058E304
 */
void ChronalVortexClass::Stop()
{
    if (m_IsActive) {
        m_IsActive = false;
        m_ToHide = false;
        m_ToDeactivate = false;
    }
}

/**
 * Performs all none rendering related vortex logic.
 *
 * 0x0058E3A8
 */
void ChronalVortexClass::AI()
{
    if (Is_Active()) {
        Movement();
        Zap_Target();

        if (m_ToDeactivate && g_GameFrame - m_ToDeactivateFrame > 50) {
            if (g_GameFrame - m_ToDeactivateFrame <= g_Scen.Get_Random_Value(0, 2000)) {
                Show();
            }

            return;
        }

        if (!m_SkipStateTick) {
            m_Frame += m_FrameIncrement;

            if (m_Frame <= 16) {
                if (m_Frame == 8) {
                    Attack();
                }

                m_SkipStateTick = !m_SkipStateTick;

                return;
            }

            m_Frame = 1;

            if (m_ToHide) {
                if (m_AIState == VORTEX_STATE_2) {
                    Set_Redraw();
                    m_Frame = 0;
                    m_IsActive = false;
                    m_SkipStateTick = !m_SkipStateTick;

                    return;
                }
            } else {
                if (!m_IsHidden) {
                    Attack();

                    if (m_AIState == VORTEX_STATE_0) {
                        m_AIState = VORTEX_STATE_1;
                    }

                    m_SkipStateTick = !m_SkipStateTick;

                    return;
                }

                if (m_AIState == VORTEX_STATE_2) {
                    Set_Redraw();

                    m_IsHidden = false;
                    m_ToDeactivate = true;
                    m_ToDeactivateFrame = g_GameFrame;

                    if (g_Scen.Get_Random_Value(0, 4) == 4) {
                        Disappear();
                    }

                    m_SkipStateTick = !m_SkipStateTick;

                    return;
                }

                Attack();
                m_AIState = VORTEX_STATE_2;
            }
        }

        m_SkipStateTick = !m_SkipStateTick;
    }
}

/**
 * Performs logic to move the vortex.
 *
 * 0x0058E510
 */
void ChronalVortexClass::Movement()
{
    int16_t new_x = Coord_Lepton_X(m_CurrentPos) + m_SpeedX;
    int16_t new_y = Coord_Lepton_Y(m_CurrentPos) + m_SpeedY;
    bool unajusted = true;

    m_CurrentPos = Coord_From_Lepton_XY(new_x, new_y);

    if (new_x >= (g_Map.Get_Map_Cell_Width() + g_Map.Get_Map_Cell_X()) * 256 - 1024) {
        unajusted = false;

        if (m_RandomSpeedX > 0) {
            m_RandomSpeedX = -m_RandomSpeedX;
        }
    }

    if (new_y > (g_Map.Get_Map_Cell_Height() + g_Map.Get_Map_Cell_Y()) * 256 - 1024) {
        unajusted = false;

        if (m_RandomSpeedY > 0) {
            m_RandomSpeedY = -m_RandomSpeedY;
        }
    }

    if (new_x <= g_Map.Get_Map_Cell_X() * 256 + 512) {
        unajusted = false;

        if (m_RandomSpeedX < 0) {
            m_RandomSpeedX = -m_RandomSpeedX;
        }
    }

    if (new_y < g_Map.Get_Map_Cell_Y() * 256 + 512) {
        unajusted = false;

        if (m_RandomSpeedY < 0) {
            m_RandomSpeedY = -m_RandomSpeedY;
        }
    }

    if (m_AIState == VORTEX_STATE_1 || m_ToDeactivate) {
        if (m_SpeedX < m_RandomSpeedX) {
            ++m_SpeedX;
        }

        if (m_SpeedX > m_RandomSpeedX) {
            --m_SpeedX;
        }

        if (m_SpeedY < m_RandomSpeedY) {
            ++m_SpeedY;
        }

        if (m_SpeedY > m_RandomSpeedY) {
            --m_SpeedY;
        }

    } else {
        if (m_SpeedX > 0) {
            m_SpeedX -= m_VortexSpeed / 8;
        }

        if (m_SpeedX < 0) {
            m_SpeedX += m_VortexSpeed / 8;
        }

        if (m_SpeedY > 0) {
            m_SpeedY -= m_VortexSpeed / 8;
        }

        if (m_SpeedY < 0) {
            m_SpeedY += m_VortexSpeed / 8;
        }
    }

    if (unajusted && g_Scen.Get_Random_Value(0, 100) == 100) {
        m_RandomSpeedX = g_Scen.Get_Random_Value(-m_VortexSpeed, m_VortexSpeed);
        m_RandomSpeedY = g_Scen.Get_Random_Value(-m_VortexSpeed, m_VortexSpeed);
    }
}

/**
 * Marks an object as the target for the vortex.
 *
 * 0x0058E780
 */
void ChronalVortexClass::Set_Target(ObjectClass *object)
{
    if (Is_Active()) {
        m_AttackState = 0;

        if (object != nullptr) {
            m_Target = object->As_Target();
            m_TargetDistance = Coord_Distance(object->Center_Coord(), m_CurrentPos);
        } else {
            m_Target = 0;
            m_TargetDistance = 0;
        }

        m_TargetSetFrame = g_GameFrame;
    }
}

/**
 * Loads the vortex object from a straw interface.
 *
 * 0x0058E324
 */
BOOL ChronalVortexClass::Load(Straw &straw)
{
    if (m_VortexGBuffer != nullptr) {
        delete m_VortexGBuffer;
        m_VortexGBuffer = nullptr;
    }

    // TODO Make load and save their members sequentially rather than dumping class
    // as one big blob.
    int total = straw.Get(this, sizeof(ChronalVortexClass));

    return (total > 0);
}

/**
 * Saves the vortex object to a pipe interface.
 *
 * 0x0058E364
 */
BOOL ChronalVortexClass::Save(Pipe &pipe)
{
    GraphicBufferClass *tmpbuff = nullptr;

    if (m_VortexGBuffer != nullptr) {
        tmpbuff = m_VortexGBuffer;
        m_VortexGBuffer = nullptr;
    }

    // TODO Make load and save their members sequentially rather than dumping class
    // as one big blob.
    int total = pipe.Put(this, sizeof(ChronalVortexClass));

    if (tmpbuff != nullptr) {
        m_VortexGBuffer = tmpbuff;
    }

    return (total > 0);
}

/**
 * Decides which object will be attacked based on range and time since last target was set.
 *
 * 0x0058E800
 */
void ChronalVortexClass::Attack()
{
    coord_t pos = Round_Coord_To_Pixel(m_CurrentPos);

    // Look through all the objects until we find one closer than 512 leptons.
    for (int index = 0; index < DisplayClass::s_Layers[LAYER_GROUND].Count(); ++index) {
        ObjectClass *objptr = DisplayClass::s_Layers[LAYER_GROUND][index];

        if (objptr->Is_Techno() && objptr->Get_Health() > 0 && Coord_Distance(objptr->Center_Coord(), pos) < 512) {
            Set_Target(objptr);
            break;
        }
    }

    // If a random interval has elapsed since target was last changed, choose a
    // closer one.
    if (Target_Legal(m_Target) && g_Scen.Get_Random_Value(0, 1000) < g_GameFrame - m_TargetSetFrame) {
        for (int index = 0; index < DisplayClass::s_Layers[LAYER_GROUND].Count(); ++index) {
            ObjectClass *objptr = DisplayClass::s_Layers[LAYER_GROUND][index];

            if (objptr != nullptr && objptr->Is_Techno()) {
                int distance = Coord_Distance(objptr->Center_Coord(), m_CurrentPos);

                // VortexRange is in leptons, so no needs to convert it now.
                if (distance < m_VortexRange && g_Scen.Get_Random_Value(0, 256) < 256) {
                    Set_Target(objptr);
                    return;
                }
            }
        }
    }
}

/**
 * Performs actual attack on current target, including animation and audio.
 *
 * 0x0058E9E0
 */
void ChronalVortexClass::Zap_Target()
{
#ifdef GAME_DLL
    void (*func)(const ChronalVortexClass *) = reinterpret_cast<void (*)(const ChronalVortexClass *)>(0x0058E9E0);
    func(this);
#elif 0 // Needs more of BuildingClass implementing correctly.
    if (!m_ToDeactivate && Target_Legal(m_Target) && m_AttackState < 1) {
        coord_t pos = Round_Coord_To_Pixel(m_CurrentPos);

        BuildingClass *tmp_bld = new BuildingClass(BUILDING_TESLA_COIL, HOUSES_GOODGUY);

        // Use a temporary building that will never be drawn to do the Zap
        if (tmp_bld != nullptr) {
            tmp_bld->Set_Coord(pos);
            ObjectClass *target = As_Object(m_Target);
            Sound_Effect(VOC_TESLA1, target->Center_Coord());
            tmp_bld->Electric_Zap(As_Target(target->Center_Coord()), 0, pos, m_RemapTable);

            delete tmp_bld;

            g_Map.Flag_To_Redraw(true);
            ++m_AttackState;

            // If target is unlucky, it takes damage.
            if (m_AttackState == 1) {
                m_AttackState = 0;
                target->Take_Damage(VortexDamage, m_TargetDistance, WARHEAD_SUPER, nullptr, true);
                m_Target = 0;
            }
        }

        if (g_Scen.Get_Random_Value(0, 2) == 2) {
            Hide();
        }
    }
#endif
}

/**
 * Renders the distortion effect for the Vortex.
 *
 * 0x0058EDD8
 */
void ChronalVortexClass::Render()
{
    if (Is_Active() && !m_ToDeactivate) {
        int frame_offset;
        char lut_name[80];

        switch (m_AIState) {
            case VORTEX_STATE_0:
                frame_offset = 0;
                break;

            case VORTEX_STATE_1:
                frame_offset = 16;
                break;

            case VORTEX_STATE_2:
                frame_offset = 32;
                break;

            default:
                break;
        }

        sprintf(lut_name, "hole%04d.lut", m_Frame + frame_offset);
        uint8_t *lut = static_cast<uint8_t *>(MixFileClass<GameFileClass>::Retrieve(lut_name));

        if (lut != nullptr) {
            if (m_VortexGBuffer == nullptr) {
                m_VortexGBuffer = new GraphicBufferClass(VORTEX_BUFF_SIZE, VORTEX_BUFF_SIZE);
            }

            // Set the logic page to our Vortex buffer to prepare for the
            // distortion. Backup the tactical window parameters and set them
            // to our vortex buffer.
            GraphicViewPortClass *oldvp = Set_Logic_Page(m_VortexGBuffer);

            int tac_x = g_WindowList[WINDOW_TACTICAL].X;
            int tac_y = g_WindowList[WINDOW_TACTICAL].Y;
            int tac_w = g_WindowList[WINDOW_TACTICAL].W;
            int tac_h = g_WindowList[WINDOW_TACTICAL].H;

            g_WindowList[WINDOW_TACTICAL].X = 0;
            g_WindowList[WINDOW_TACTICAL].Y = 0;
            g_WindowList[WINDOW_TACTICAL].W = m_VortexGBuffer->Get_Width();
            g_WindowList[WINDOW_TACTICAL].H = m_VortexGBuffer->Get_Height();

            // Redraw the current the terrain for the current map position into
            // our VortexGBuffer to apply the distortion to.
            for (int y = 0; y < 4; ++y) {
                for (int x = 0; x < 4; ++x) {
                    coord_t cellnum = Cell_From_XY(x + Coord_Cell_X(m_CurrentPos), y + Coord_Cell_Y(m_CurrentPos));

                    CellClass &cell = g_Map[cellnum];
                    TemplateType type = cell.Get_Template();

                    int icon_num;
                    TemplateTypeClass *tt;

                    if (type != TEMPLATE_NONE && type != 255 && type >= TEMPLATE_FIRST) {
                        tt = &TemplateTypeClass::As_Reference(type);
                        icon_num = cell.Get_Icon();
                    } else {
                        tt = &TemplateTypeClass::As_Reference(TEMPLATE_CLEAR);
                        icon_num = cell.Clear_Icon();
                    }

                    if (tt->Get_Image_Data() != nullptr) {
                        Buffer_Draw_Stamp_Clip(*m_VortexGBuffer,
                            (IconControlType *)tt->Get_Image_Data(),
                            icon_num,
                            x * CELL_PIXELS,
                            y * CELL_PIXELS,
                            nullptr,
                            g_WindowList[WINDOW_0].X,
                            g_WindowList[WINDOW_0].Y,
                            g_WindowList[WINDOW_0].W,
                            g_WindowList[WINDOW_0].H);
                    }

                    if (cell.Get_Smudge() != SMUDGE_NONE && cell.Get_Smudge_Frame() != -1) {
                        SmudgeTypeClass::As_Reference(cell.Get_Smudge()).Draw_It(x * CELL_PIXELS, y * CELL_PIXELS, cell.Get_Smudge_Frame());
                    }

                    if (cell.Get_Overlay() != OVERLAY_NONE && cell.Get_Overlay_Frame() != -1) {
                        OverlayTypeClass::As_Reference(cell.Get_Overlay()).Draw_It(x * CELL_PIXELS, y * CELL_PIXELS, cell.Get_Overlay_Frame());
                    }
                }
            }

            // Restore old settings.
            Set_Logic_Page(oldvp);
            g_WindowList[WINDOW_TACTICAL].X = tac_x;
            g_WindowList[WINDOW_TACTICAL].Y = tac_y;
            g_WindowList[WINDOW_TACTICAL].W = tac_w;
            g_WindowList[WINDOW_TACTICAL].H = tac_h;

            // Perform the distortion based on the lut we loaded.
            Coordinate_Remap(m_VortexGBuffer,
                Lepton_To_Pixel(Coord_Lepton_X(Coord_Sub_Cell(m_CurrentPos))),
                Lepton_To_Pixel(Coord_Lepton_Y(Coord_Sub_Cell(m_CurrentPos))),
                64,
                64,
                lut);

            // Create a viewport based on the tactical dimensions.
            GraphicViewPortClass vp(g_LogicPage->Get_Graphic_Buffer(),
                0,
                g_LogicPage->Get_YPos() + 16, // + Tab height
                Lepton_To_Pixel(g_Map.Get_DisplayWidth()),
                Lepton_To_Pixel(g_Map.Get_DisplayHeight()));

            // Do a lot of maths to clip the positions for the blit to the
            // tactical viewport we created.
            int16_t vtx_xpos = Lepton_Round_To_Pixel(Coord_Cell_To_Lepton(Coord_Sub_Cell_X(m_CurrentPos)))
                - Lepton_Round_To_Pixel(g_Map.Get_DisplayWidth());
            int16_t vtx_ypos = Lepton_Round_To_Pixel(Coord_Cell_To_Lepton(Coord_Sub_Cell_Y(m_CurrentPos)))
                - Lepton_Round_To_Pixel(g_Map.Get_DisplayHeight());

            int s_xpos = 0;
            int s_ypos = 0;
            int d_xpos = Lepton_To_Pixel(vtx_xpos);
            int d_ypos = Lepton_To_Pixel(vtx_ypos);
            int blt_w = VORTEX_BUFF_SIZE;
            int blt_h = VORTEX_BUFF_SIZE;

            if (d_xpos < 0) {
                blt_w = d_xpos + VORTEX_BUFF_SIZE;
                s_xpos -= d_xpos;
                d_xpos = 0;
            }

            if (d_ypos < 0) {
                blt_h = d_ypos + VORTEX_BUFF_SIZE;
                s_ypos -= d_ypos;
            }

            if (d_xpos + blt_w > vp.Get_Width()) {
                blt_w -= d_xpos + blt_w - vp.Get_Width();
            }

            if (d_ypos + blt_h > vp.Get_Height()) {
                blt_h -= d_ypos + blt_h - vp.Get_Height();
            }

            // If the vortex is actually in the destination view port, blit it
            // across to it.
            if (blt_h > 0 && blt_w > 0) {
                m_VortexGBuffer->Blit(vp, s_xpos, s_ypos, d_xpos, d_ypos, blt_w, blt_h, false);
            }
        }
    }
}

/**
 * Performs distortion based on the lookup table data passed in.
 * 
 * @param vp ViewPort to apply the distortion to. 
 * @param x X axis position to apply the distortion at.
 * @param y Y axis position to apply the distortion at.
 * @param w Width of the distorted area.
 * @param h Height of the distorted area.
 * @param lut Lookup table that contains the transformation offsets.
 *
 * 0x0058EBC4
 */
void ChronalVortexClass::Coordinate_Remap(GraphicViewPortClass *vp, int x, int y, int w, int h, uint8_t *lut)
{
    BufferClass buff(w * h);

    if (vp->Lock()) {
        int pitch = vp->Get_Full_Pitch();
        uint8_t *src = static_cast<uint8_t *>(vp->Get_Offset()) + pitch * y;
        uint8_t *dst = buff.Get_Buffer();

        // LUT format has X and Y offsets into the source followed by which of
        // 4 ghost tables to map the pixel to.
        for (int i = y; i < h; ++i) {
            for (int j = x; j < w; ++j) {
                uint8_t *src_off = lut[0] + src;
                *dst++ = m_GhostTable[lut[2]][src_off[pitch * lut[1]]];
                lut += LUT_ENTRY_SIZE;
            }
        }

        vp->From_Buffer(x, y, w, h, buff.Get_Buffer());
        vp->Unlock();
    }
}

/**
 * Flags the area of the map that the vortex occupies to redraw.
 *
 * 0x0058F67C
 */
void ChronalVortexClass::Set_Redraw()
{
    if (Is_Active()) {
        int x_pos = Coord_Cell_X(m_CurrentPos);
        int y_pos = Coord_Cell_Y(m_CurrentPos);

        for (int y = std::max(0, y_pos - 1); y < y_pos + 4; ++y) {
            for (int x = std::max(0, x_pos - 1); x < x_pos + 4; ++x) {
                g_Map[Cell_From_XY(x, y)].Redraw_Objects();
            }
        }
    }
}

/**
 * Generates a "fading" lookup table specifically for the Chronal Vortex.
 *
 * @param palette Palette to generate the table against.
 * @param fade_table Pointer to the memory region that will store the table.
 * @param color Colour that the fade will be towards.
 * @param frac Fraction that the initial palette colour should be adjusted toward the target colour. Maths is frac/256.
 * @param h Height of the distorted area.
 * @param lut Lookup table that contains the transformation offsets.
 *
 * @see Conquer_Build_Fading_Table()
 * @see Build_Fading_Table()
 * @see Make_Fading_Table()
 *
 * 0x0058F858
 */
void ChronalVortexClass::Build_Fading_Table(PaletteClass &palette, void *fade_table, int color, int frac)
{
    if (fade_table != nullptr) {
        uint8_t *dst = static_cast<uint8_t *>(fade_table);
        int index = -1;
        RGBClass *target = &palette[color];

        // Iterate through the palette, skip certain special indecies and adjust
        // others to be index that is closest to current entry adjusted toward
        // target by frac amount.
        for (int i = 0; i < GHOST_TABLE_SIZE; ++i) {
            if (i != 0 && (i < 96 || i >= 103) && i != 255 && i != 103) {
                RGBClass tmp = palette[i];
                tmp.Adjust(frac, *target);
                int prevdiff = 0;

                // Iterate over palette looking for nearest color to our adjusted
                // tmp palette entry.
                for (int j = 0; j < GHOST_TABLE_SIZE; ++j) {
                    if (j != 0 && (j < 96 || j >= 103) && i != 255 && i != 103) {
                        int diff = palette[j].Difference(tmp);

                        if (index == -1 || diff < prevdiff) {
                            index = j;
                            prevdiff = diff;
                        }
                    }
                }

                *dst++ = index;
            } else {
                *dst++ = i;
            }
        }
    }
}

/**
 * Loads or generates the fading and remap tables used by the vortex drawing.
 * 
 * 0x0058F724
 */
void ChronalVortexClass::Setup_Remap_Tables(TheaterType theater)
{
    char buffer[16];

    if (theater != m_Theater) {
        m_Theater = theater;
        snprintf(buffer, sizeof(buffer), "%s_vtx.pal", g_Theaters[m_Theater].ext);
        DEBUG_LOG("Building Chrono Vortex RemapTable for theater %s (%s)\n", Name_From_Theater(m_Theater), buffer);
        GameFileClass fc(buffer);

        // Read the table if available, otherwise we generate it and write it.
        if (fc.Is_Available()) {
            fc.Read(m_GhostTable, sizeof(m_GhostTable));
        } else {
            for (unsigned i = 0; i < GHOST_TABLE_COUNT; ++i) {
                int frac = 240 - ((i * 256) / 16);
                Build_Fading_Table(g_GamePalette, m_GhostTable[i], 0, frac);
            }

            fc.Write(m_GhostTable, sizeof(m_GhostTable));
        }
    }

    // Set up 1 to 1 mapping for RemapTable
    for (int i = 0; i < GHOST_TABLE_SIZE; ++i) {
        m_RemapTable[i] = i;
    }

    // Adjust certain indicies to be remapped.
    m_RemapTable[192] = 208;
    m_RemapTable[193] = 209;
    m_RemapTable[194] = 210;
    m_RemapTable[195] = 211;
    m_RemapTable[196] = 212;
    m_RemapTable[197] = 213;
    m_RemapTable[198] = 214;
    m_RemapTable[199] = 215;
}

/**
 * Stops the vortex from pursuing its current target.
 * 
 * @param target Packed target pointer of the object to stop pursuing.
 * 
 * 0x0058F960
 */
void ChronalVortexClass::Detach(target_t target)
{
    if (Target_Legal(target) && m_Target == target) {
        Set_Target(nullptr);
    }
}
