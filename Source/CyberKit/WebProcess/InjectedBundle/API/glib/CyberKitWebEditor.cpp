/*
 * Copyright (C) 2015 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CyberKitWebEditor.h"

#include "CyberKitWebEditorPrivate.h"
#include "CyberKitWebPagePrivate.h"
#include <wtf/glib/WTFGType.h>

using namespace CyberKit;
using namespace CyberCore;

/**
 * SECTION: CyberKitWebEditor
 * @Short_description: Access to editing capabilities of a #CyberKitWebPage
 * @Title: CyberKitWebEditor
 * @See_also: #CyberKitWebPage
 *
 * The CyberKitWebEditor provides access to various editing capabilities of
 * a #CyberKitWebPage such as a possibility to react to the current selection in
 * #CyberKitWebPage.
 *
 * Since: 2.10
 */
enum {
    SELECTION_CHANGED,

    LAST_SIGNAL
};

struct _CyberKitWebEditorPrivate {
    CyberKitWebPage* webPage;
};

static guint signals[LAST_SIGNAL] = { 0, };

WEBKIT_DEFINE_TYPE(CyberKitWebEditor, webkit_web_editor, G_TYPE_OBJECT)

static void webkit_web_editor_class_init(CyberKitWebEditorClass* klass)
{
    /**
     * CyberKitWebEditor::selection-changed:
     * @editor: the #CyberKitWebEditor on which the signal is emitted
     *
     * This signal is emitted for every selection change inside a #CyberKitWebPage
     * as well as for every caret position change as the caret is a collapsed
     * selection.
     *
     * Since: 2.10
     */
    signals[SELECTION_CHANGED] = g_signal_new(
        "selection-changed",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0, nullptr, nullptr,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);
}

class PageEditorClient final : public API::InjectedBundle::EditorClient {
public:
    explicit PageEditorClient(CyberKitWebEditor* editor)
        : m_editor(editor)
    {
    }

private:
    void didChangeSelection(WebPage&, const String&) final
    {
        g_signal_emit(m_editor, signals[SELECTION_CHANGED], 0);
    }

    CyberKitWebEditor* m_editor;
};

CyberKitWebEditor* webkitWebEditorCreate(CyberKitWebPage* webPage)
{
    CyberKitWebEditor* editor = WEBKIT_WEB_EDITOR(g_object_new(WEBKIT_TYPE_WEB_EDITOR, nullptr));
    editor->priv->webPage = webPage;
    webkitWebPageGetPage(webPage)->setInjectedBundleEditorClient(makeUnique<PageEditorClient>(editor));
    return editor;
}

/**
 * webkit_web_editor_get_page:
 * @editor: a #CyberKitWebEditor
 *
 * Gets the #CyberKitWebPage that is associated with the #CyberKitWebEditor that can
 * be used to access the #CyberKitDOMDocument currently loaded into it.
 *
 * Returns: (transfer none): the associated #CyberKitWebPage
 *
 * Since: 2.10
 */
CyberKitWebPage* webkit_web_editor_get_page(CyberKitWebEditor* editor)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_EDITOR(editor), nullptr);

    return editor->priv->webPage;
}
