/* Whether the browser is Chromium-based with MojoJS enabled */
export const isChromiumBased = 'MojoInterfaceInterceptor' in self;

/* Whether the browser is CyberKit-based with internal test-only API enabled */
export const isCyberKitBased = !isChromiumBased && 'internals' in self;
