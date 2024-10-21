#include "eq_parser.h"

/**
 * @brief Get the type of the cartesian graph
 * @param equation The equation
 * @return cartesian_graph_type_e
 */
cartesian_graph_type_e eq_parser_get_type(char *equation)
{
    if (equation == NULL)
    {
        return NONE;
    }
    if (strstr(equation, "x^2") != NULL)
    {
        return PARABOLA;
    }
    else if (strstr(equation, "x") != NULL)
    {
        return STRAIGHT;
    }
    return NONE;
}

/**
 * @brief Get the coefficients of a straight equation
 * @param equation The equation
 * @param m The slope
 * @param q The y-intercept
 */
void eq_parser_get_straight_coefficients(char *equation, float *m, float *q)
{
    char *x_pos = strstr(equation, "x");

    if (x_pos != NULL)
    {
        char *m_part = strndup(equation, x_pos - equation);
        if (strlen(m_part) == 0 || strcmp(m_part, " ") == 0)
        {
            *m = 1;
        }
        else
        {
            *m = atof(m_part);
        }
        free(m_part);

        char *q_part = strstr(x_pos, "+");
        if (q_part != NULL)
        {
            *q = atof(q_part + 1);
        }
        else
        {
            q_part = strstr(x_pos, "-");
            if (q_part != NULL)
            {
                *q = atof(q_part);
            }
            else
            {
                *q = 0;
            }
        }
    }
}

/**
 * @brief Get the coefficients of a parabola equation
 * @param equation The equation
 * @param a The coefficient of x^2
 * @param b The coefficient of x
 * @param c The constant term
 */
void eq_parser_get_parabola_coefficients(char *equation, float *a, float *b, float *c)
{
    // Inizializza i coefficienti
    *a = *b = *c = 0;

    // Trova la posizione di "x^2"
    char *x2_pos = strstr(equation, "x^2");
    if (x2_pos != NULL)
    {
        // Estrai la parte per il coefficiente "a"
        char *a_part = strndup(equation, x2_pos - equation);
        if (strlen(a_part) == 0 || strcmp(a_part, " ") == 0)
        {
            *a = 1; // Se non c'è coefficiente scritto, allora è 1
        }
        else if (strcmp(a_part, "-") == 0)
        {
            *a = -1; // Se c'è solo "-", il coefficiente è -1
        }
        else
        {
            *a = atof(a_part); // Converti il coefficiente "a"
        }
        free(a_part);
    }

    // Trova la posizione di "x"
    char *x_pos = strstr(equation, "x");
    if (x_pos != NULL && x_pos > x2_pos) // Assicurati che non sia "x^2"
    {
        // Estrai la parte per il coefficiente "b"
        char *b_part = strndup(x2_pos + 3, x_pos - (x2_pos + 3)); // Salta "x^2"
        if (strlen(b_part) == 0 || strcmp(b_part, " ") == 0)
        {
            *b = 1; // Se non c'è coefficiente scritto, allora è 1
        }
        else if (strcmp(b_part, "-") == 0)
        {
            *b = -1; // Se c'è solo "-", il coefficiente è -1
        }
        else
        {
            *b = atof(b_part); // Converti il coefficiente "b"
        }
        free(b_part);
    }

    // Estrai il coefficiente "c"
    char *c_part = strstr(equation, "+");
    if (c_part != NULL)
    {
        *c = atof(c_part + 1);
    }
    else
    {
        c_part = strstr(equation, "-");
        if (c_part != NULL)
        {
            *c = atof(c_part); // Gestisce anche il caso in cui "c" è negativo
        }
        else
        {
            *c = 0; // Se non c'è nessun termine libero, "c" è 0
        }
    }
}
