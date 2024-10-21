GrB_Monoid max_monoid;
GrB_Monoid_new(&max_monoid, GrB_MAX_INT32, (int32_t)0);

GrB_Vector_reduce_INT32(&max_value, NULL, max_monoid, C, NULL);
printf("Number of colors: %d\n", max_value);

GrB_Monoid_free(&max_monoid);