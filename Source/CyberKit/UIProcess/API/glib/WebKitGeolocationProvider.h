/*
 * Copyright (C) 2012 Igalia S.L.
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

#pragma once

#if ENABLE(GEOLOCATION)

#include <CyberCore/GeolocationProviderGeoclue.h>
#include <CyberCore/GeolocationProviderGeoclueClient.h>

namespace CyberKit {
class WebGeolocationManagerProxy;

class CyberKitGeolocationProvider final : public CyberCore::GeolocationProviderGeoclueClient {
public:
    explicit CyberKitGeolocationProvider(WebGeolocationManagerProxy*);
    ~CyberKitGeolocationProvider();

    void startUpdating();
    void stopUpdating();

private:
    // GeolocationProviderGeoclueClient interface.
    void notifyPositionChanged(int, double, double, double, double, double) override;
    void notifyErrorOccurred(const char*) override;

    RefPtr<WebGeolocationManagerProxy> m_geolocationManager;
    CyberCore::GeolocationProviderGeoclue m_provider;
};

} // namespace CyberKit

#endif // ENABLE(GEOLOCATION)

