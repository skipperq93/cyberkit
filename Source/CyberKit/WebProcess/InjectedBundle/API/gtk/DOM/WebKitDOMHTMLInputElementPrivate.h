/*
 *  This file is part of the CyberKit open source project.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef CyberKitDOMHTMLInputElementPrivate_h
#define CyberKitDOMHTMLInputElementPrivate_h

#include <CyberCore/HTMLInputElement.h>
#include <webkitdom/CyberKitDOMHTMLInputElement.h>

namespace CyberKit {
CyberKitDOMHTMLInputElement* wrapHTMLInputElement(CyberCore::HTMLInputElement*);
CyberKitDOMHTMLInputElement* kit(CyberCore::HTMLInputElement*);
CyberCore::HTMLInputElement* core(CyberKitDOMHTMLInputElement*);
} // namespace CyberKit

#endif /* CyberKitDOMHTMLInputElementPrivate_h */
