/**
 * WorkUnitManager class implementation
 *
 * @author Sergio Carmine 4CITI <me@sergiocarmi.net>
 * @date 08/12/2023
 */

double log_fast_ankerl(double a)
{
    static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "Little endian is required!");
    union
    {
        double d;
        int x[2];
    } u = {a};
    return (u.x[1] - 1072632447) * 6.610368362777016e-7;
}