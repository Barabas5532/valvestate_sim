import("stdfaust.lib");

/*
 * Looks up values from the table with linear interpolation between entries.
 *
 * Parameters:
 *   table_size - the table size
 *   table_data - the table content
 *   sample - the current sample. Will be clipped so it fits the range 0 to 1.
 *
 * Returns the value of sample from the table where 0 is the first entry, 1 is
 * the last entry, and other values are linear interpolations of the two nearast
 * values from the table.
*/
linearInterpolateTable(tableSize, tableData, sample) = out
   with {
        clippedSample = max(0.0, min(1.0, sample));
        pos = clippedSample * (tableSize - 1);

        idx1 = int(pos);
        idx2 = min(tableSize - 1, idx1 + 1);
        frac = pos - float(idx1);

        value1 = rdtable(tableSize, tableData, idx1);
        value2 = rdtable(tableSize, tableData, idx2);

        out = value1 + frac * (value2 - value1);
    } : _;
