/*
 * This file is part of the AdvanceMAME project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * In addition, as a special exception, Andrea Mazzoleni
 * gives permission to link the code of this program with
 * the MAME library (or with modified versions of MAME that use the
 * same license as MAME), and distribute linked combinations including
 * the two.  You must obey the GNU General Public License in all
 * respects for all of the code used other than MAME.  If you modify
 * this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to
 * do so, delete this exception statement from your version.
 */

#ifndef __MAME2_H
#define __MAME2_H

#if defined(MESS)

#include "../../srcmess/mame.h"
#include "../../srcmess/driver.h"
#include "../../srcmess/info.h"
#include "../../srcmess/osdepend.h"
#include "../../srcmess/ui_text.h"

#elif defined(PAC)

#include "../../srcpac/mame.h"
#include "../../srcpac/driver.h"
#include "../../srcpac/info.h"
#include "../../srcpac/osdepend.h"
#include "../../srcpac/ui_text.h"

#else

#include "../../src/mame.h"
#include "../../src/driver.h"
#include "../../src/info.h"
#include "../../src/osdepend.h"
#include "../../src/ui_text.h"

#endif

#endif

