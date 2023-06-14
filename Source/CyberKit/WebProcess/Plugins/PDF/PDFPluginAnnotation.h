/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PDFPluginAnnotation_h
#define PDFPluginAnnotation_h

#if ENABLE(PDFKIT_PLUGIN)

#include <CyberCore/EventListener.h>
#include <wtf/RefCounted.h>
#include <wtf/RetainPtr.h>

namespace CyberCore {
class Document;
class Element;
}

OBJC_CLASS PDFAnnotation;
OBJC_CLASS PDFLayerController;

namespace CyberKit {

class PDFPlugin;

class PDFPluginAnnotation : public RefCounted<PDFPluginAnnotation> {
public:
    static RefPtr<PDFPluginAnnotation> create(PDFAnnotation *, PDFLayerController *, PDFPlugin*);
    virtual ~PDFPluginAnnotation();

    CyberCore::Element* element() const { return m_element.get(); }
    PDFAnnotation *annotation() const { return m_annotation.get(); }
    PDFPlugin* plugin() const { return m_plugin; }

    virtual void updateGeometry();
    virtual void commit();

    void attach(CyberCore::Element*);

protected:
    PDFPluginAnnotation(PDFAnnotation *annotation, PDFLayerController *pdfLayerController, PDFPlugin* plugin)
        : m_parent(0)
        , m_annotation(annotation)
        , m_eventListener(PDFPluginAnnotationEventListener::create(this))
        , m_pdfLayerController(pdfLayerController)
        , m_plugin(plugin)
    {
    }

    CyberCore::Element* parent() const { return m_parent; }
    PDFLayerController *pdfLayerController() const { return m_pdfLayerController; }
    CyberCore::EventListener* eventListener() const { return m_eventListener.get(); }

    virtual bool handleEvent(CyberCore::Event&);

private:
    virtual Ref<CyberCore::Element> createAnnotationElement() = 0;

    class PDFPluginAnnotationEventListener : public CyberCore::EventListener {
    public:
        static Ref<PDFPluginAnnotationEventListener> create(PDFPluginAnnotation* annotation)
        {
            return adoptRef(*new PDFPluginAnnotationEventListener(annotation));
        }

        bool operator==(const EventListener& listener) const override { return this == &listener; }

        void setAnnotation(PDFPluginAnnotation* annotation) { m_annotation = annotation; }

    private:

        PDFPluginAnnotationEventListener(PDFPluginAnnotation* annotation)
            : CyberCore::EventListener(CyberCore::EventListener::CPPEventListenerType)
            , m_annotation(annotation)
        {
        }

        void handleEvent(CyberCore::ScriptExecutionContext&, CyberCore::Event&) override;

        PDFPluginAnnotation* m_annotation;
    };

    CyberCore::Element* m_parent;

    RefPtr<CyberCore::Element> m_element;
    RetainPtr<PDFAnnotation> m_annotation;

    RefPtr<PDFPluginAnnotationEventListener> m_eventListener;

    PDFLayerController *m_pdfLayerController;
    PDFPlugin* m_plugin;
};

} // namespace CyberKit

#endif // ENABLE(PDFKIT_PLUGIN)

#endif // PDFPluginAnnotation_h
