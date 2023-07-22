/*
 * Copyright (C) 2013 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "CyberKitDOMHTMLPrivate.h"

#include <CyberCore/HTMLAnchorElement.h>
#include <CyberCore/HTMLAppletElement.h>
#include <CyberCore/HTMLAreaElement.h>
#include <CyberCore/HTMLAudioElement.h>
#include <CyberCore/HTMLBRElement.h>
#include <CyberCore/HTMLBaseElement.h>
#include <CyberCore/HTMLBodyElement.h>
#include <CyberCore/HTMLButtonElement.h>
#include <CyberCore/HTMLCanvasElement.h>
#include <CyberCore/HTMLDListElement.h>
#include <CyberCore/HTMLDirectoryElement.h>
#include <CyberCore/HTMLDivElement.h>
#include <CyberCore/HTMLElement.h>
#include <CyberCore/HTMLEmbedElement.h>
#include <CyberCore/HTMLFieldSetElement.h>
#include <CyberCore/HTMLFontElement.h>
#include <CyberCore/HTMLFormElement.h>
#include <CyberCore/HTMLFrameElement.h>
#include <CyberCore/HTMLFrameSetElement.h>
#include <CyberCore/HTMLHRElement.h>
#include <CyberCore/HTMLHeadElement.h>
#include <CyberCore/HTMLHeadingElement.h>
#include <CyberCore/HTMLHtmlElement.h>
#include <CyberCore/HTMLIFrameElement.h>
#include <CyberCore/HTMLImageElement.h>
#include <CyberCore/HTMLInputElement.h>
#include <CyberCore/HTMLKeygenElement.h>
#include <CyberCore/HTMLLIElement.h>
#include <CyberCore/HTMLLabelElement.h>
#include <CyberCore/HTMLLegendElement.h>
#include <CyberCore/HTMLLinkElement.h>
#include <CyberCore/HTMLMapElement.h>
#include <CyberCore/HTMLMarqueeElement.h>
#include <CyberCore/HTMLMenuElement.h>
#include <CyberCore/HTMLMetaElement.h>
#include <CyberCore/HTMLModElement.h>
#include <CyberCore/HTMLNames.h>
#include <CyberCore/HTMLOListElement.h>
#include <CyberCore/HTMLObjectElement.h>
#include <CyberCore/HTMLOptGroupElement.h>
#include <CyberCore/HTMLOptionElement.h>
#include <CyberCore/HTMLParagraphElement.h>
#include <CyberCore/HTMLParamElement.h>
#include <CyberCore/HTMLPreElement.h>
#include <CyberCore/HTMLQuoteElement.h>
#include <CyberCore/HTMLScriptElement.h>
#include <CyberCore/HTMLSelectElement.h>
#include <CyberCore/HTMLStyleElement.h>
#include <CyberCore/HTMLTableCaptionElement.h>
#include <CyberCore/HTMLTableCellElement.h>
#include <CyberCore/HTMLTableColElement.h>
#include <CyberCore/HTMLTableElement.h>
#include <CyberCore/HTMLTableRowElement.h>
#include <CyberCore/HTMLTableSectionElement.h>
#include <CyberCore/HTMLTextAreaElement.h>
#include <CyberCore/HTMLTitleElement.h>
#include <CyberCore/HTMLUListElement.h>
#include <CyberCore/HTMLVideoElement.h>
#include "CyberKitDOMHTMLAnchorElementPrivate.h"
#include "CyberKitDOMHTMLAppletElementPrivate.h"
#include "CyberKitDOMHTMLAreaElementPrivate.h"
#include "CyberKitDOMHTMLBRElementPrivate.h"
#include "CyberKitDOMHTMLBaseElementPrivate.h"
#include "CyberKitDOMHTMLBodyElementPrivate.h"
#include "CyberKitDOMHTMLButtonElementPrivate.h"
#include "CyberKitDOMHTMLCanvasElementPrivate.h"
#include "CyberKitDOMHTMLDListElementPrivate.h"
#include "CyberKitDOMHTMLDirectoryElementPrivate.h"
#include "CyberKitDOMHTMLDivElementPrivate.h"
#include "CyberKitDOMHTMLElementPrivate.h"
#include "CyberKitDOMHTMLEmbedElementPrivate.h"
#include "CyberKitDOMHTMLFieldSetElementPrivate.h"
#include "CyberKitDOMHTMLFontElementPrivate.h"
#include "CyberKitDOMHTMLFormElementPrivate.h"
#include "CyberKitDOMHTMLFrameElementPrivate.h"
#include "CyberKitDOMHTMLFrameSetElementPrivate.h"
#include "CyberKitDOMHTMLHRElementPrivate.h"
#include "CyberKitDOMHTMLHeadElementPrivate.h"
#include "CyberKitDOMHTMLHeadingElementPrivate.h"
#include "CyberKitDOMHTMLHtmlElementPrivate.h"
#include "CyberKitDOMHTMLIFrameElementPrivate.h"
#include "CyberKitDOMHTMLImageElementPrivate.h"
#include "CyberKitDOMHTMLInputElementPrivate.h"
#include "CyberKitDOMHTMLLIElementPrivate.h"
#include "CyberKitDOMHTMLLabelElementPrivate.h"
#include "CyberKitDOMHTMLLegendElementPrivate.h"
#include "CyberKitDOMHTMLLinkElementPrivate.h"
#include "CyberKitDOMHTMLMapElementPrivate.h"
#include "CyberKitDOMHTMLMarqueeElementPrivate.h"
#include "CyberKitDOMHTMLMenuElementPrivate.h"
#include "CyberKitDOMHTMLMetaElementPrivate.h"
#include "CyberKitDOMHTMLModElementPrivate.h"
#include "CyberKitDOMHTMLOListElementPrivate.h"
#include "CyberKitDOMHTMLObjectElementPrivate.h"
#include "CyberKitDOMHTMLOptGroupElementPrivate.h"
#include "CyberKitDOMHTMLOptionElementPrivate.h"
#include "CyberKitDOMHTMLParagraphElementPrivate.h"
#include "CyberKitDOMHTMLParamElementPrivate.h"
#include "CyberKitDOMHTMLPreElementPrivate.h"
#include "CyberKitDOMHTMLQuoteElementPrivate.h"
#include "CyberKitDOMHTMLScriptElementPrivate.h"
#include "CyberKitDOMHTMLSelectElementPrivate.h"
#include "CyberKitDOMHTMLStyleElementPrivate.h"
#include "CyberKitDOMHTMLTableCaptionElementPrivate.h"
#include "CyberKitDOMHTMLTableCellElementPrivate.h"
#include "CyberKitDOMHTMLTableColElementPrivate.h"
#include "CyberKitDOMHTMLTableElementPrivate.h"
#include "CyberKitDOMHTMLTableRowElementPrivate.h"
#include "CyberKitDOMHTMLTableSectionElementPrivate.h"
#include "CyberKitDOMHTMLTextAreaElementPrivate.h"
#include "CyberKitDOMHTMLTitleElementPrivate.h"
#include "CyberKitDOMHTMLUListElementPrivate.h"

G_GNUC_BEGIN_IGNORE_DEPRECATIONS;

namespace CyberKit {

using namespace CyberCore;
using namespace CyberCore::HTMLNames;

// macro(TagName, ElementName)

#define FOR_EACH_HTML_TAG(macro) \
    macro(a, Anchor) \
    macro(applet, Applet) \
    macro(area, Area) \
    macro(base, Base) \
    macro(blockquote, Quote) \
    macro(body, Body) \
    macro(br, BR) \
    macro(button, Button) \
    macro(canvas, Canvas) \
    macro(caption, TableCaption) \
    macro(col, TableCol) \
    macro(del, Mod) \
    macro(dir, Directory) \
    macro(div, Div) \
    macro(dl, DList) \
    macro(embed, Embed) \
    macro(fieldset, FieldSet) \
    macro(font, Font) \
    macro(form, Form) \
    macro(frame, Frame) \
    macro(frameset, FrameSet) \
    macro(h1, Heading) \
    macro(head, Head) \
    macro(hr, HR) \
    macro(html, Html) \
    macro(iframe, IFrame) \
    macro(img, Image) \
    macro(input, Input) \
    macro(label, Label) \
    macro(legend, Legend) \
    macro(li, LI) \
    macro(link, Link) \
    macro(map, Map) \
    macro(marquee, Marquee) \
    macro(menu, Menu) \
    macro(meta, Meta) \
    macro(object, Object) \
    macro(ol, OList) \
    macro(optgroup, OptGroup) \
    macro(option, Option) \
    macro(p, Paragraph) \
    macro(param, Param) \
    macro(pre, Pre) \
    macro(q, Quote) \
    macro(script, Script) \
    macro(select, Select) \
    macro(style, Style) \
    macro(table, Table) \
    macro(tbody, TableSection) \
    macro(td, TableCell) \
    macro(textarea, TextArea) \
    macro(title, Title) \
    macro(tr, TableRow) \
    macro(ul, UList) \
    macro(colgroup, TableCol) \
    macro(h2, Heading) \
    macro(h3, Heading) \
    macro(h4, Heading) \
    macro(h5, Heading) \
    macro(h6, Heading) \
    macro(image, Image) \
    macro(ins, Mod) \
    macro(listing, Pre) \
    macro(tfoot, TableSection) \
    macro(th, TableCell) \
    macro(thead, TableSection) \
    macro(xmp, Pre)

#define DEFINE_HTML_WRAPPER(TagName, ElementName) \
    static CyberKitDOMHTMLElement* TagName##Wrapper(HTMLElement* element) \
    { \
        return WEBKIT_DOM_HTML_ELEMENT(wrapHTML##ElementName##Element(static_cast<HTML##ElementName##Element*>(element))); \
    }
    FOR_EACH_HTML_TAG(DEFINE_HTML_WRAPPER)
#undef DEFINE_HTML_WRAPPER

typedef CyberKitDOMHTMLElement* (*HTMLElementWrapFunction)(HTMLElement*);

CyberKitDOMHTMLElement* wrap(HTMLElement* element)
{
    static HashMap<const QualifiedName::QualifiedNameImpl*, HTMLElementWrapFunction> map;
    if (map.isEmpty()) {
#define ADD_HTML_WRAPPER(TagName, ElementName) map.set(TagName##Tag->impl(), TagName##Wrapper);
        FOR_EACH_HTML_TAG(ADD_HTML_WRAPPER)
#undef ADD_HTML_WRAPPER
    }

    if (HTMLElementWrapFunction wrapFunction = map.get(element->tagQName().impl()))
        return wrapFunction(element);

    return wrapHTMLElement(element);
}

}
G_GNUC_END_IGNORE_DEPRECATIONS;
