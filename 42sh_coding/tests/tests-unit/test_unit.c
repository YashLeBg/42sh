#include <criterion/criterion.h>
#include <criterion/logging.h>

#include <stddef.h>
#include <string.h>


// tests simple pour voir si criterion criterionne bien 



Test(sanity, criterion_runs)
{
    cr_assert(1);
}

Test(sanity, basic_math)
{
    cr_assert_eq(2 + 67, 69);
    cr_assert_neq(2 + 67, 100);
}

Test(sanity, string_compare)
{
    cr_assert_str_eq("42sh", "42sh");
    cr_assert_str_neq("42sh", "ash");
}



// je rajoute de suite les tests j'ai juste besoin de la nouvelle version du code pour voir vos fonctions etc etc etc ...
