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
    _B_TYPE_END
};