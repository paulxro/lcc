#pragma once

enum TokenType {
    /* Keywords. */
    _K_TYPE_BEGIN,
    k_exit,             /* 'exit' keyword, e.g. exit(0) */
    k_func,             /* 'fn'   keyword, e.g. fn [name] ... */
    k_let,
    k_if,
    _K_TYPE_END,

    /* Operators */
    _O_TYPE_BEGIN,
    o_plus,
    o_sub,
    o_equal,
    o_equal_equal,
    _O_TYPE_END,

    /* Data Types. */
    _D_TYPE_BEGIN,
    d_int,
    _D_TYPE_END,

    /* Literals. */
    _L_TYPE_BEGIN,
    l_int,
    l_float,
    _L_TYPE_END,

    /* Misc. */
    _M_TYPE_BEGIN,
    m_ident,
    m_keybreak,
    m_unknown,
    _M_TYPE_END,

    /* Token breaks. */
    _B_TYPE_BEGIN,
    b_lparen,
    b_rparen,
    b_colon,
    b_semi,
    b_left_curl,
    b_right_curl,
    b_comma,
    _B_TYPE_END
};

inline const char* to_string(TokenType type) {
    switch (type) {
        case k_exit:        return "k_exit";
        case k_func:        return "k_func";
        case k_let:         return "k_let";
        case k_if:          return "k_if";

        case o_plus:        return "o_plus";
        case o_sub:         return "o_sub";
        case o_equal:       return "o_equal";
        case o_equal_equal: return "o_equal_equal";

        case d_int:         return "d_int";

        case l_int:         return "l_int";
        case l_float:       return "l_float";

        case m_ident:       return "m_ident";
        case m_keybreak:    return "m_keybreak";
        case m_unknown:     return "m_unknown";

        case b_lparen:      return "b_lparen";
        case b_rparen:      return "b_rparen";
        case b_colon:       return "b_colon";
        case b_semi:        return "b_semi";
        case b_left_curl:   return "b_left_curl";
        case b_right_curl:  return "b_right_curl";
        case b_comma:       return "b_comma";

        default:            return "unknown_token_type";
    }
}
