/*
 *  Copyright (C) 2010, 2011 Igalia S.L.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include <CyberCore/AddEventListenerOptions.h>
#include <CyberCore/EventListener.h>
#include <CyberCore/EventTarget.h>
#include <wtf/RefPtr.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/text/CString.h>

typedef struct _GObject GObject;
typedef struct _GClosure GClosure;

namespace CyberKit {

class GObjectEventListener : public CyberCore::EventListener {
public:

    static bool addEventListener(GObject* target, CyberCore::EventTarget* coreTarget, const char* domEventName, GClosure* handler, bool useCapture)
    {
        Ref<GObjectEventListener> listener(adoptRef(*new GObjectEventListener(target, coreTarget, domEventName, handler, useCapture)));
        return coreTarget->addEventListener(AtomString::fromLatin1(domEventName), WTFMove(listener), useCapture);
    }

    static bool removeEventListener(GObject* target, CyberCore::EventTarget* coreTarget, const char* domEventName, GClosure* handler, bool useCapture)
    {
        GObjectEventListener key(target, coreTarget, domEventName, handler, useCapture);
        return coreTarget->removeEventListener(AtomString::fromLatin1(domEventName), key, useCapture);
    }

    static void gobjectDestroyedCallback(GObjectEventListener* listener, GObject*)
    {
        listener->gobjectDestroyed();
    }

    static const GObjectEventListener* cast(const CyberCore::EventListener* listener)
    {
        return listener->type() == GObjectEventListenerType
            ? static_cast<const GObjectEventListener*>(listener)
            : nullptr;
    }

    bool operator==(const CyberCore::EventListener& other) const override;

private:
    GObjectEventListener(GObject*, CyberCore::EventTarget*, const char* domEventName, GClosure*, bool capture);
    ~GObjectEventListener();
    void gobjectDestroyed();

    void handleEvent(CyberCore::ScriptExecutionContext&, CyberCore::Event&) override;

    GObject* m_target;
    // We do not need to keep a reference to the m_coreTarget, because
    // we only use it when the GObject and thus the m_coreTarget object is alive.
    CyberCore::EventTarget* m_coreTarget;
    CString m_domEventName;
    GRefPtr<GClosure> m_handler;
    bool m_capture;
};

} // namespace CyberKit

