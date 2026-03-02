#pragma once

#define for_each(iter, items, num_items) for (typeof (&items[0])(iter) = &(items)[0]; (iter) != &(items)[(num_items)]; iter++)
