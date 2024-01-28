#pragma once

namespace LibGL::Rendering
{
    enum class EBlendFactor
    {
        ZERO                     = 0,		// Factor is equal to 0.
        ONE                      = 1,		// Factor is equal to 1.
        SRC_COLOR                = 0x0300,	// Factor is equal to the source color vector.
        ONE_MINUS_SRC_COLOR      = 0x0301,	// Factor is equal to 1 minus the source color vector.
        SRC_ALPHA                = 0x0302,	// Factor is equal to the alpha component of the source color vector.
        ONE_MINUS_SRC_ALPHA      = 0x0303,	// Factor is equal to 1-alpha of the source color vector.
        DST_ALPHA                = 0x0304,	// Factor is equal to the alpha component of the destination color vector.
        ONE_MINUS_DST_ALPHA      = 0x0305,	// Factor is equal to 1-alpha of the destination color vector.
        DST_COLOR                = 0x0306,	// Factor is equal to the destination color vector.
        ONE_MINUS_DST_COLOR      = 0x0307,	// Factor is equal to 1 minus the destination color vector.
        CONSTANT_COLOR           = 0x8001,	// Factor is equal to the constant color vector.
        ONE_MINUS_CONSTANT_COLOR = 0x8002,	// Factor is equal to 1 - the constant color vector.
        CONSTANT_ALPHA           = 0x8003,	// Factor is equal to the alpha component of the constant color vector.
        ONE_MINUS_CONSTANT_ALPHA = 0x8004,	// Factor is equal to 1-alpha of the constant color vector.
    };
}
