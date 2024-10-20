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