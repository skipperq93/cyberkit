/*
 * Copyright (C) 2015 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
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
#include "CyberKitConsoleMessage.h"

#include "CyberKitConsoleMessagePrivate.h"

G_DEFINE_BOXED_TYPE(CyberKitConsoleMessage, webkit_console_message, webkit_console_message_copy, webkit_console_message_free)

/**
 * webkit_console_message_copy:
 * @console_message: a #CyberKitConsoleMessage
 *
 * Make a copy of @console_message.
 *
 * Returns: (transfer full): A copy of passed in #CyberKitConsoleMessage
 *
 * Since: 2.12
 */
CyberKitConsoleMessage* webkit_console_message_copy(CyberKitConsoleMessage* consoleMessage)
{
    g_return_val_if_fail(consoleMessage, nullptr);
    CyberKitConsoleMessage* copy = static_cast<CyberKitConsoleMessage*>(fastZeroedMalloc(sizeof(CyberKitConsoleMessage)));
    new (copy) CyberKitConsoleMessage(consoleMessage);
    return copy;
}

/**
 * webkit_console_message_free:
 * @console_message: a #CyberKitConsoleMessage
 *
 * Free the #CyberKitConsoleMessage
 *
 * Since: 2.12
 */
void webkit_console_message_free(CyberKitConsoleMessage* consoleMessage)
{
    g_return_if_fail(consoleMessage);
    consoleMessage->~CyberKitConsoleMessage();
    fastFree(consoleMessage);
}

/**
 * webkit_console_message_get_source:
 * @console_message: a #CyberKitConsoleMessage
 *
 * Gets the source of a #CyberKitConsoleMessage
 *
 * Returns: a #CyberKitConsoleMessageSource indicating the source of @console_message
 *
 * Since: 2.12
 */
CyberKitConsoleMessageSource webkit_console_message_get_source(CyberKitConsoleMessage* consoleMessage)
{
    g_return_val_if_fail(consoleMessage, WEBKIT_CONSOLE_MESSAGE_SOURCE_OTHER);
    switch (consoleMessage->source) {
    case JSC::MessageSource::JS:
        return WEBKIT_CONSOLE_MESSAGE_SOURCE_JAVASCRIPT;
    case JSC::MessageSource::Network:
        return WEBKIT_CONSOLE_MESSAGE_SOURCE_NETWORK;
    case JSC::MessageSource::ConsoleAPI:
        return WEBKIT_CONSOLE_MESSAGE_SOURCE_CONSOLE_API;
    case JSC::MessageSource::Security:
        return WEBKIT_CONSOLE_MESSAGE_SOURCE_SECURITY;
    case JSC::MessageSource::Other:
    default:
        break;
    }

    return WEBKIT_CONSOLE_MESSAGE_SOURCE_OTHER;
}

/**
 * webkit_console_message_get_level:
 * @console_message: a #CyberKitConsoleMessage
 *
 * Gets the log level of a #CyberKitConsoleMessage
 *
 * Returns: a #CyberKitConsoleMessageLevel indicating the log level of @console_message
 *
 * Since: 2.12
 */
CyberKitConsoleMessageLevel webkit_console_message_get_level(CyberKitConsoleMessage* consoleMessage)
{
    g_return_val_if_fail(consoleMessage, WEBKIT_CONSOLE_MESSAGE_LEVEL_LOG);
    switch (consoleMessage->level) {
    case JSC::MessageLevel::Log:
        return WEBKIT_CONSOLE_MESSAGE_LEVEL_LOG;
    case JSC::MessageLevel::Warning:
        return WEBKIT_CONSOLE_MESSAGE_LEVEL_WARNING;
    case JSC::MessageLevel::Error:
        return WEBKIT_CONSOLE_MESSAGE_LEVEL_ERROR;
    case JSC::MessageLevel::Debug:
        return WEBKIT_CONSOLE_MESSAGE_LEVEL_DEBUG;
    case JSC::MessageLevel::Info:
        return WEBKIT_CONSOLE_MESSAGE_LEVEL_INFO;
    }

    ASSERT_NOT_REACHED();
    return WEBKIT_CONSOLE_MESSAGE_LEVEL_LOG;
}

/**
 * webkit_console_message_get_text:
 * @console_message: a #CyberKitConsoleMessage
 *
 * Gets the text message of a #CyberKitConsoleMessage
 *
 * Returns: the text message of @console_message
 *
 * Since: 2.12
 */
const gchar* webkit_console_message_get_text(CyberKitConsoleMessage* consoleMessage)
{
    g_return_val_if_fail(consoleMessage, nullptr);
    return consoleMessage->message.data();
}

/**
 * webkit_console_message_get_line:
 * @console_message: a #CyberKitConsoleMessage
 *
 * Gets the line number of a #CyberKitConsoleMessage
 *
 * Returns: the line number of @console_message
 *
 * Since: 2.12
 */
guint webkit_console_message_get_line(CyberKitConsoleMessage* consoleMessage)
{
    g_return_val_if_fail(consoleMessage, 0);
    return consoleMessage->lineNumber;
}

/**
 * webkit_console_message_get_source_id:
 * @console_message: a #CyberKitConsoleMessage
 *
 * Gets the source identifier of a #CyberKitConsoleMessage
 *
 * Returns: the source identifier of @console_message
 *
 * Since: 2.12
 */
const gchar* webkit_console_message_get_source_id(CyberKitConsoleMessage* consoleMessage)
{
    g_return_val_if_fail(consoleMessage, nullptr);
    return consoleMessage->sourceID.data();
}
