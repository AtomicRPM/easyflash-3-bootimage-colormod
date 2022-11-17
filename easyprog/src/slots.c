/*
 * EasyProg - filedlg.c - File open dialog
 *
 * (c) 2011 Thomas Giesel
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Thomas Giesel skoe@directbox.com
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "easyprog.h"
#include "eapiglue.h"
#include "texts.h"
#include "flash.h"
#include "slots.h"
#include "screen.h"
#include "selectbox.h"
#include "util.h"

#define MAX_KERNALS 8
#define MAX_ARS     2
#define MAX_FREEZER 4

uint8_t g_nSelectedSlot;
uint8_t g_nSlots;

// copy of EF directory in RAM
static efmenu_dir_t m_EFDir;

static const char* m_pEFSignature = "EF-Directory V1:";
static const char* m_pEFSignature2 = "EF-Directory V2:";

/******************************************************************************/
/**
 * Read the directory from EasyFlash to m_EFDir. If there is no valid
 * directory in the cartridge, initialize the structure with defaults.
 */

void slotsFillEFDir(void)
{
    uint8_t i, nSlot;

    nSlot = g_nSelectedSlot;
    g_nSelectedSlot = EF_DIR_SLOT;

    eapiReInit();
    eapiSetBank(EF_DIR_BANK);

    efCopyCartROM(&m_EFDir, (void*)(0x8000), sizeof(m_EFDir));
    if (memcmp(m_EFDir.signature,
               m_pEFSignature2, sizeof(m_EFDir.signature)) != 0)
    {
        // initialize new directory
        strcpy(utilStr, "Replay Slot 1");
        strcpy(m_EFDir.freezers[0], utilStr);
        strcpy(utilStr, "Replay Slot 2");
        strcpy(m_EFDir.freezers[1], utilStr);
        strcpy(utilStr, "SS5 Slot");
        strcpy(m_EFDir.freezers[2], utilStr);
        strcpy(utilStr, "FC3 Slot");
        strcpy(m_EFDir.freezers[3], utilStr);

        if (memcmp(m_EFDir.signature,
                   m_pEFSignature, sizeof(m_EFDir.signature)) != 0)
        {
            for (i = 1; i < EF_DIR_NUM_SLOTS; ++i)
            {
                strcpy(utilStr, "EF Slot ");
                utilAppendDecimal(i);
                strcpy(m_EFDir.slots[i], utilStr);
            }
            for (i = 0; i < EF_DIR_NUM_KERNALS; ++i)
            {
                strcpy(utilStr, "KERNAL ");
                utilAppendDecimal(i + 1);
                strcpy(m_EFDir.kernals[i], utilStr);
            }
        }

        memcpy(m_EFDir.signature,
                m_pEFSignature2, sizeof(m_EFDir.signature));
        m_EFDir.checksum = 0x4711;
    }

    // slot 0 always gets this name
    strcpy(m_EFDir.slots[0], "System Area");

    g_nSelectedSlot = nSlot;
}

/******************************************************************************/
/**
 * Let the user select a slot. Return the slot number.
 * Return 0xff if the user canceled the selection.
 */
uint8_t __fastcall__ selectSlotDialog(void)
{
	SelectBoxEntry entries[FLASH_MAX_SLOTS + 1];
   SelectBoxEntry* pEntry;
  uint8_t    nSlot, rv;

    slotsFillEFDir();
   // termination for strings with strlen() == EF_DIR_ENTRY_SIZE
   // and termination for list
    memset(entries, 0, (FLASH_MAX_SLOTS + 1) * sizeof(SelectBoxEntry));

    for (nSlot = 0; nSlot < g_nSlots; ++nSlot)
    {
        pEntry = entries + nSlot;
        // take care: target must be at least as large as source
       memcpy(pEntry->label, m_EFDir.slots[nSlot],
               sizeof(m_EFDir.slots[0]));
        // empty slots get a '-' because the menu needs a string
        if (pEntry->label[0] == 0)
            pEntry->label[0] = '-';
    }

    rv = selectBox(entries, "a slot");
    return rv;
}
/******************************************************************************/
/**
 * Let the user select a KERNAL slot. Return the slot number.
 * Return 0xff if the user canceled the selection.
 */
uint8_t selectKERNALSlotDialog(void)
{
    SelectBoxEntry  entries[MAX_KERNALS + 1];
    SelectBoxEntry* pEntry;
    char*           pLabel;
    uint8_t         nSlot, rv;

    slotsFillEFDir();
    // termination for strings with strlen() == EF_DIR_ENTRY_SIZE
    // and termination for list
    memset(entries, 0, (FLASH_MAX_SLOTS + 1) * sizeof(SelectBoxEntry));

    pEntry = entries;
    pLabel = m_EFDir.kernals[0];
    for (nSlot = 1; nSlot <= MAX_KERNALS; ++nSlot)
    {
        // take care: target must be at least as large as source
        memcpy(pEntry->label, pLabel, sizeof(m_EFDir.kernals[0]));
        // empty slots get a '-' because the menu needs a string
        if (pEntry->label[0] == 0)
            pEntry->label[0] = '-';

        ++pEntry;
        pLabel += sizeof(m_EFDir.kernals[0]);
    }

    rv = selectBox(entries, "a KERNAL slot");
    return rv;
}

/******************************************************************************/
/**
 * Let the user select a Freezer slot. Return the slot number.
 * Return 0xff if the user canceled the selection.
 */

uint8_t selectFreezerSlotDialog(void)
{
    SelectBoxEntry  entries[MAX_FREEZER + 1];
    SelectBoxEntry* pEntry;
    char*           pLabel;
    uint8_t         nSlot, rv;

    slotsFillEFDir();
    // termination for strings with strlen() == EF_DIR_ENTRY_SIZE
    // and termination for list
    memset(entries, 0, (MAX_FREEZER + 1) * sizeof(SelectBoxEntry));

    pEntry = entries;
    pLabel = m_EFDir.freezers[0];
    for (nSlot = 1; nSlot <= MAX_FREEZER; ++nSlot)
    {
        // take care: target must be at least as large as source
        memcpy(pEntry->label, pLabel, sizeof(m_EFDir.freezers[0]));
        // empty slots get a '-' because the menu needs a string
        if (pEntry->label[0] == 0)
            pEntry->label[0] = '-';

        ++pEntry;
        pLabel += sizeof(m_EFDir.freezers[0]);
    }

    rv = selectBox(entries, "a Freezer slot");
    return rv;
}

/******************************************************************************/
/**
 * Let the user select a slot type. Return the slot type.
 * Return 0xff if the user canceled the selection.
 */
uint8_t selectSlotTypeDialog(void)
{
   const SelectBoxEntry aEntries[4] =
    {
            { "EasyFlash", 0 },   // EF_SLOTS
            { "KERNAL", 0 },      // KERNAL_SLOTS
            { "Freezer", 0 },    // FREEZER_SLOTS
            { "", 0 }

    };
    uint8_t rv;

    rv = selectBox(aEntries, "slot type");
    return rv;

}

/******************************************************************************/
/**
 * If we have more than one slot, ask the user which one he wants to use.
 *
 * Return 0 if the user canceled the selection.
 **/
uint8_t __fastcall__ checkAskForSlot(void)
{
    uint8_t s;

    if (g_nSlots > 1)
    {
        for (;;)
        {
            refreshMainScreen();
            s = selectSlotDialog();
            if (s == 0xff)
                return 0;

            g_nSelectedSlot = s;
            if (g_nSelectedSlot == 0)
            {
                if (screenPrintDialog(apStrSlot0,
                        BUTTON_ENTER | BUTTON_STOP) == BUTTON_ENTER)
                    break;
            }
            else
                break;
        }
        refreshMainScreen();
    }
    return 1;
}

/******************************************************************************/
/**
 * This sets g_nSelectedSlot and refreshes the main screen, but does not
 * write to the I/O register yet.
 **/
void __fastcall__ slotSelect(uint8_t slot)
{
    g_nSelectedSlot = slot;
    if (g_nSlots > 1)
    {
        refreshMainScreen();
    }
}


/******************************************************************************/
/**
 * Read the slot directory from flash, set the name of an EF slot or a KERNAL
 * in the slot directory and write it back to flash.
 *
 * If nKERNAL is 0xff, the name is written to EF Slot number g_nSelectedSlot.
 * Otherwise nKERNAL contains the KERNAL slot number.
 *
 **/
 
void __fastcall__ slotSaveName(const char* name, uint8_t nKERNAL, uint8_t nFreezer)
{
    EasyFlashAddr addr;
    uint8_t  nSlot;

    nSlot = g_nSelectedSlot;
    g_nSelectedSlot = EF_DIR_SLOT;

    slotsFillEFDir();
    if (nKERNAL != 0xff)
        strncpy(m_EFDir.kernals[nKERNAL], name, sizeof(m_EFDir.kernals[0]));
    else if (nFreezer != 0xff)
        strncpy(m_EFDir.freezers[nFreezer], name, sizeof(m_EFDir.freezers[0]));
    else
        strncpy(m_EFDir.slots[nSlot], name, sizeof(m_EFDir.slots[0]));

    addr.nSlot = EF_DIR_SLOT;
    addr.nBank = EF_DIR_BANK;
    addr.nChip = 0;
    addr.nOffset = 0;
    // slotsFillEFDir initialized EAPI etc. for us already
    eraseSector(EF_DIR_BANK, 0);
    do
    {
        memcpy(BLOCK_BUFFER, ((uint8_t*) &m_EFDir) + addr.nOffset, 256);
        flashWriteBlock(&addr);
        addr.nOffset += 256;
    }
    while (addr.nOffset < sizeof(m_EFDir));
    g_nSelectedSlot = nSlot;
}

void slotsEditDirectory(void)
{
    uint8_t nType, nSlot, nKERNAL, nFreezer;

    nType = selectSlotTypeDialog();
    if (nType == 0xff)
        return;

    for (;;)
    {
        if (nType == EF_SLOTS)
        {
            nKERNAL = 0xff;
            nFreezer = 0xff;
            nSlot = selectSlotDialog();
            if (nSlot == 0xff)
                return;
            g_nSelectedSlot = nSlot;

            if (g_nSelectedSlot == 0)
            {
                screenPrintSimpleDialog(apStrSlot0NoDir);
                continue; // urks!
            }
            else
            {
                memset(utilStr, 0, UTIL_STR_SIZE);
                memcpy(utilStr, m_EFDir.slots[g_nSelectedSlot], EF_DIR_ENTRY_SIZE);
            }
        }
        else if (nType == KERNAL_SLOTS)
        {
            nFreezer = 0xff;
            nKERNAL = selectKERNALSlotDialog();
            if (nKERNAL == 0xff)
                return;

            memset(utilStr, 0, UTIL_STR_SIZE);
            memcpy(utilStr, m_EFDir.kernals[nKERNAL], EF_DIR_ENTRY_SIZE);
        }
        else
        {
            nKERNAL = 0xff;
            nFreezer = selectFreezerSlotDialog();
            if (nFreezer == 0xff)
                return;

            memset(utilStr, 0, UTIL_STR_SIZE);
            memcpy(utilStr, m_EFDir.freezers[nFreezer], EF_DIR_ENTRY_SIZE);
        }

        slotSaveName(screenReadInput("Name", utilStr), nKERNAL, nFreezer);
    }
}

