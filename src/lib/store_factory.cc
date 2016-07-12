#include <stdexcept>
#include "store.h"
#include "store_factory.h"
#include "store_spec.h"
#include "query.h"

BaseStore* StoreFactory::Create(StoreSpec& store_spec) {

  size_t dims_count = store_spec.DimsCount();
  size_t metrics_count = store_spec.MetricsCount();

  switch(dims_count) {
#if DIMS_X == 1
    case 1: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<1,1>(store_spec);
              case 2: return new Store<1,2>(store_spec);
              case 3: return new Store<1,3>(store_spec);
              case 4: return new Store<1,4>(store_spec);
              case 5: return new Store<1,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<1,6>(store_spec);
              case 7: return new Store<1,7>(store_spec);
              case 8: return new Store<1,8>(store_spec);
              case 9: return new Store<1,9>(store_spec);
              case 10: return new Store<1,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<1,11>(store_spec);
              case 12: return new Store<1,12>(store_spec);
              case 13: return new Store<1,13>(store_spec);
              case 14: return new Store<1,14>(store_spec);
              case 15: return new Store<1,15>(store_spec);
#endif
            }

    case 2: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<2,1>(store_spec);
              case 2: return new Store<2,2>(store_spec);
              case 3: return new Store<2,3>(store_spec);
              case 4: return new Store<2,4>(store_spec);
              case 5: return new Store<2,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<2,6>(store_spec);
              case 7: return new Store<2,7>(store_spec);
              case 8: return new Store<2,8>(store_spec);
              case 9: return new Store<2,9>(store_spec);
              case 10: return new Store<2,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<2,11>(store_spec);
              case 12: return new Store<2,12>(store_spec);
              case 13: return new Store<2,13>(store_spec);
              case 14: return new Store<2,14>(store_spec);
              case 15: return new Store<2,15>(store_spec);
#endif
            }

    case 3: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<3,1>(store_spec);
              case 2: return new Store<3,2>(store_spec);
              case 3: return new Store<3,3>(store_spec);
              case 4: return new Store<3,4>(store_spec);
              case 5: return new Store<3,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<3,6>(store_spec);
              case 7: return new Store<3,7>(store_spec);
              case 8: return new Store<3,8>(store_spec);
              case 9: return new Store<3,9>(store_spec);
              case 10: return new Store<3,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<3,11>(store_spec);
              case 12: return new Store<3,12>(store_spec);
              case 13: return new Store<3,13>(store_spec);
              case 14: return new Store<3,14>(store_spec);
              case 15: return new Store<3,15>(store_spec);
#endif
            }

    case 4: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<4,1>(store_spec);
              case 2: return new Store<4,2>(store_spec);
              case 3: return new Store<4,3>(store_spec);
              case 4: return new Store<4,4>(store_spec);
              case 5: return new Store<4,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<4,6>(store_spec);
              case 7: return new Store<4,7>(store_spec);
              case 8: return new Store<4,8>(store_spec);
              case 9: return new Store<4,9>(store_spec);
              case 10: return new Store<4,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<4,11>(store_spec);
              case 12: return new Store<4,12>(store_spec);
              case 13: return new Store<4,13>(store_spec);
              case 14: return new Store<4,14>(store_spec);
              case 15: return new Store<4,15>(store_spec);
#endif
            }

    case 5: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<5,1>(store_spec);
              case 2: return new Store<5,2>(store_spec);
              case 3: return new Store<5,3>(store_spec);
              case 4: return new Store<5,4>(store_spec);
              case 5: return new Store<5,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<5,6>(store_spec);
              case 7: return new Store<5,7>(store_spec);
              case 8: return new Store<5,8>(store_spec);
              case 9: return new Store<5,9>(store_spec);
              case 10: return new Store<5,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<5,11>(store_spec);
              case 12: return new Store<5,12>(store_spec);
              case 13: return new Store<5,13>(store_spec);
              case 14: return new Store<5,14>(store_spec);
              case 15: return new Store<5,15>(store_spec);
#endif
            }

    case 6: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<6,1>(store_spec);
              case 2: return new Store<6,2>(store_spec);
              case 3: return new Store<6,3>(store_spec);
              case 4: return new Store<6,4>(store_spec);
              case 5: return new Store<6,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<6,6>(store_spec);
              case 7: return new Store<6,7>(store_spec);
              case 8: return new Store<6,8>(store_spec);
              case 9: return new Store<6,9>(store_spec);
              case 10: return new Store<6,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<6,11>(store_spec);
              case 12: return new Store<6,12>(store_spec);
              case 13: return new Store<6,13>(store_spec);
              case 14: return new Store<6,14>(store_spec);
              case 15: return new Store<6,15>(store_spec);
#endif
            }

    case 7: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<7,1>(store_spec);
              case 2: return new Store<7,2>(store_spec);
              case 3: return new Store<7,3>(store_spec);
              case 4: return new Store<7,4>(store_spec);
              case 5: return new Store<7,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<7,6>(store_spec);
              case 7: return new Store<7,7>(store_spec);
              case 8: return new Store<7,8>(store_spec);
              case 9: return new Store<7,9>(store_spec);
              case 10: return new Store<7,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<7,11>(store_spec);
              case 12: return new Store<7,12>(store_spec);
              case 13: return new Store<7,13>(store_spec);
              case 14: return new Store<7,14>(store_spec);
              case 15: return new Store<7,15>(store_spec);
#endif
            }

    case 8: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<8,1>(store_spec);
              case 2: return new Store<8,2>(store_spec);
              case 3: return new Store<8,3>(store_spec);
              case 4: return new Store<8,4>(store_spec);
              case 5: return new Store<8,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<8,6>(store_spec);
              case 7: return new Store<8,7>(store_spec);
              case 8: return new Store<8,8>(store_spec);
              case 9: return new Store<8,9>(store_spec);
              case 10: return new Store<8,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<8,11>(store_spec);
              case 12: return new Store<8,12>(store_spec);
              case 13: return new Store<8,13>(store_spec);
              case 14: return new Store<8,14>(store_spec);
              case 15: return new Store<8,15>(store_spec);
#endif
            }

    case 9: switch(metrics_count) {
#if METRICS_X == 1
              case 1: return new Store<9,1>(store_spec);
              case 2: return new Store<9,2>(store_spec);
              case 3: return new Store<9,3>(store_spec);
              case 4: return new Store<9,4>(store_spec);
              case 5: return new Store<9,5>(store_spec);
#elif METRICS_X == 2
              case 6: return new Store<9,6>(store_spec);
              case 7: return new Store<9,7>(store_spec);
              case 8: return new Store<9,8>(store_spec);
              case 9: return new Store<9,9>(store_spec);
              case 10: return new Store<9,10>(store_spec);
#elif METRICS_X == 3
              case 11: return new Store<9,11>(store_spec);
              case 12: return new Store<9,12>(store_spec);
              case 13: return new Store<9,13>(store_spec);
              case 14: return new Store<9,14>(store_spec);
              case 15: return new Store<9,15>(store_spec);
#endif
            }

    case 10: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<10,1>(store_spec);
               case 2: return new Store<10,2>(store_spec);
               case 3: return new Store<10,3>(store_spec);
               case 4: return new Store<10,4>(store_spec);
               case 5: return new Store<10,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<10,6>(store_spec);
               case 7: return new Store<10,7>(store_spec);
               case 8: return new Store<10,8>(store_spec);
               case 9: return new Store<10,9>(store_spec);
               case 10: return new Store<10,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<10,11>(store_spec);
               case 12: return new Store<10,12>(store_spec);
               case 13: return new Store<10,13>(store_spec);
               case 14: return new Store<10,14>(store_spec);
               case 15: return new Store<10,15>(store_spec);
#endif
             }

#elif DIMS_X == 2
    case 11: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<11,1>(store_spec);
               case 2: return new Store<11,2>(store_spec);
               case 3: return new Store<11,3>(store_spec);
               case 4: return new Store<11,4>(store_spec);
               case 5: return new Store<11,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<11,6>(store_spec);
               case 7: return new Store<11,7>(store_spec);
               case 8: return new Store<11,8>(store_spec);
               case 9: return new Store<11,9>(store_spec);
               case 10: return new Store<11,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<11,11>(store_spec);
               case 12: return new Store<11,12>(store_spec);
               case 13: return new Store<11,13>(store_spec);
               case 14: return new Store<11,14>(store_spec);
               case 15: return new Store<11,15>(store_spec);
#endif
             }

    case 12: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<12,1>(store_spec);
               case 2: return new Store<12,2>(store_spec);
               case 3: return new Store<12,3>(store_spec);
               case 4: return new Store<12,4>(store_spec);
               case 5: return new Store<12,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<12,6>(store_spec);
               case 7: return new Store<12,7>(store_spec);
               case 8: return new Store<12,8>(store_spec);
               case 9: return new Store<12,9>(store_spec);
               case 10: return new Store<12,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<12,11>(store_spec);
               case 12: return new Store<12,12>(store_spec);
               case 13: return new Store<12,13>(store_spec);
               case 14: return new Store<12,14>(store_spec);
               case 15: return new Store<12,15>(store_spec);
#endif
             }

    case 13: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<13,1>(store_spec);
               case 2: return new Store<13,2>(store_spec);
               case 3: return new Store<13,3>(store_spec);
               case 4: return new Store<13,4>(store_spec);
               case 5: return new Store<13,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<13,6>(store_spec);
               case 7: return new Store<13,7>(store_spec);
               case 8: return new Store<13,8>(store_spec);
               case 9: return new Store<13,9>(store_spec);
               case 10: return new Store<13,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<13,11>(store_spec);
               case 12: return new Store<13,12>(store_spec);
               case 13: return new Store<13,13>(store_spec);
               case 14: return new Store<13,14>(store_spec);
               case 15: return new Store<13,15>(store_spec);
#endif
             }

    case 14: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<14,1>(store_spec);
               case 2: return new Store<14,2>(store_spec);
               case 3: return new Store<14,3>(store_spec);
               case 4: return new Store<14,4>(store_spec);
               case 5: return new Store<14,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<14,6>(store_spec);
               case 7: return new Store<14,7>(store_spec);
               case 8: return new Store<14,8>(store_spec);
               case 9: return new Store<14,9>(store_spec);
               case 10: return new Store<14,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<14,11>(store_spec);
               case 12: return new Store<14,12>(store_spec);
               case 13: return new Store<14,13>(store_spec);
               case 14: return new Store<14,14>(store_spec);
               case 15: return new Store<14,15>(store_spec);
#endif
             }

    case 15: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<15,1>(store_spec);
               case 2: return new Store<15,2>(store_spec);
               case 3: return new Store<15,3>(store_spec);
               case 4: return new Store<15,4>(store_spec);
               case 5: return new Store<15,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<15,6>(store_spec);
               case 7: return new Store<15,7>(store_spec);
               case 8: return new Store<15,8>(store_spec);
               case 9: return new Store<15,9>(store_spec);
               case 10: return new Store<15,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<15,11>(store_spec);
               case 12: return new Store<15,12>(store_spec);
               case 13: return new Store<15,13>(store_spec);
               case 14: return new Store<15,14>(store_spec);
               case 15: return new Store<15,15>(store_spec);
#endif
             }

    case 16: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<16,1>(store_spec);
               case 2: return new Store<16,2>(store_spec);
               case 3: return new Store<16,3>(store_spec);
               case 4: return new Store<16,4>(store_spec);
               case 5: return new Store<16,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<16,6>(store_spec);
               case 7: return new Store<16,7>(store_spec);
               case 8: return new Store<16,8>(store_spec);
               case 9: return new Store<16,9>(store_spec);
               case 10: return new Store<16,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<16,11>(store_spec);
               case 12: return new Store<16,12>(store_spec);
               case 13: return new Store<16,13>(store_spec);
               case 14: return new Store<16,14>(store_spec);
               case 15: return new Store<16,15>(store_spec);
#endif
             }

    case 17: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<17,1>(store_spec);
               case 2: return new Store<17,2>(store_spec);
               case 3: return new Store<17,3>(store_spec);
               case 4: return new Store<17,4>(store_spec);
               case 5: return new Store<17,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<17,6>(store_spec);
               case 7: return new Store<17,7>(store_spec);
               case 8: return new Store<17,8>(store_spec);
               case 9: return new Store<17,9>(store_spec);
               case 10: return new Store<17,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<17,11>(store_spec);
               case 12: return new Store<17,12>(store_spec);
               case 13: return new Store<17,13>(store_spec);
               case 14: return new Store<17,14>(store_spec);
               case 15: return new Store<17,15>(store_spec);
#endif
             }

    case 18: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<18,1>(store_spec);
               case 2: return new Store<18,2>(store_spec);
               case 3: return new Store<18,3>(store_spec);
               case 4: return new Store<18,4>(store_spec);
               case 5: return new Store<18,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<18,6>(store_spec);
               case 7: return new Store<18,7>(store_spec);
               case 8: return new Store<18,8>(store_spec);
               case 9: return new Store<18,9>(store_spec);
               case 10: return new Store<18,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<18,11>(store_spec);
               case 12: return new Store<18,12>(store_spec);
               case 13: return new Store<18,13>(store_spec);
               case 14: return new Store<18,14>(store_spec);
               case 15: return new Store<18,15>(store_spec);
#endif
             }

    case 19: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<19,1>(store_spec);
               case 2: return new Store<19,2>(store_spec);
               case 3: return new Store<19,3>(store_spec);
               case 4: return new Store<19,4>(store_spec);
               case 5: return new Store<19,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<19,6>(store_spec);
               case 7: return new Store<19,7>(store_spec);
               case 8: return new Store<19,8>(store_spec);
               case 9: return new Store<19,9>(store_spec);
               case 10: return new Store<19,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<19,11>(store_spec);
               case 12: return new Store<19,12>(store_spec);
               case 13: return new Store<19,13>(store_spec);
               case 14: return new Store<19,14>(store_spec);
               case 15: return new Store<19,15>(store_spec);
#endif
             }

#elif DIMS_X == 3
    case 20: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<20,1>(store_spec);
               case 2: return new Store<20,2>(store_spec);
               case 3: return new Store<20,3>(store_spec);
               case 4: return new Store<20,4>(store_spec);
               case 5: return new Store<20,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<20,6>(store_spec);
               case 7: return new Store<20,7>(store_spec);
               case 8: return new Store<20,8>(store_spec);
               case 9: return new Store<20,9>(store_spec);
               case 10: return new Store<20,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<20,11>(store_spec);
               case 12: return new Store<20,12>(store_spec);
               case 13: return new Store<20,13>(store_spec);
               case 14: return new Store<20,14>(store_spec);
               case 15: return new Store<20,15>(store_spec);
#endif
             }

    case 21: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<21,1>(store_spec);
               case 2: return new Store<21,2>(store_spec);
               case 3: return new Store<21,3>(store_spec);
               case 4: return new Store<21,4>(store_spec);
               case 5: return new Store<21,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<21,6>(store_spec);
               case 7: return new Store<21,7>(store_spec);
               case 8: return new Store<21,8>(store_spec);
               case 9: return new Store<21,9>(store_spec);
               case 10: return new Store<21,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<21,11>(store_spec);
               case 12: return new Store<21,12>(store_spec);
               case 13: return new Store<21,13>(store_spec);
               case 14: return new Store<21,14>(store_spec);
               case 15: return new Store<21,15>(store_spec);
#endif
             }

    case 22: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<22,1>(store_spec);
               case 2: return new Store<22,2>(store_spec);
               case 3: return new Store<22,3>(store_spec);
               case 4: return new Store<22,4>(store_spec);
               case 5: return new Store<22,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<22,6>(store_spec);
               case 7: return new Store<22,7>(store_spec);
               case 8: return new Store<22,8>(store_spec);
               case 9: return new Store<22,9>(store_spec);
               case 10: return new Store<22,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<22,11>(store_spec);
               case 12: return new Store<22,12>(store_spec);
               case 13: return new Store<22,13>(store_spec);
               case 14: return new Store<22,14>(store_spec);
               case 15: return new Store<22,15>(store_spec);
#endif
             }

    case 23: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<23,1>(store_spec);
               case 2: return new Store<23,2>(store_spec);
               case 3: return new Store<23,3>(store_spec);
               case 4: return new Store<23,4>(store_spec);
               case 5: return new Store<23,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<23,6>(store_spec);
               case 7: return new Store<23,7>(store_spec);
               case 8: return new Store<23,8>(store_spec);
               case 9: return new Store<23,9>(store_spec);
               case 10: return new Store<23,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<23,11>(store_spec);
               case 12: return new Store<23,12>(store_spec);
               case 13: return new Store<23,13>(store_spec);
               case 14: return new Store<23,14>(store_spec);
               case 15: return new Store<23,15>(store_spec);
#endif
             }

    case 24: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<24,1>(store_spec);
               case 2: return new Store<24,2>(store_spec);
               case 3: return new Store<24,3>(store_spec);
               case 4: return new Store<24,4>(store_spec);
               case 5: return new Store<24,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<24,6>(store_spec);
               case 7: return new Store<24,7>(store_spec);
               case 8: return new Store<24,8>(store_spec);
               case 9: return new Store<24,9>(store_spec);
               case 10: return new Store<24,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<24,11>(store_spec);
               case 12: return new Store<24,12>(store_spec);
               case 13: return new Store<24,13>(store_spec);
               case 14: return new Store<24,14>(store_spec);
               case 15: return new Store<24,15>(store_spec);
#endif
             }

    case 25: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<25,1>(store_spec);
               case 2: return new Store<25,2>(store_spec);
               case 3: return new Store<25,3>(store_spec);
               case 4: return new Store<25,4>(store_spec);
               case 5: return new Store<25,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<25,6>(store_spec);
               case 7: return new Store<25,7>(store_spec);
               case 8: return new Store<25,8>(store_spec);
               case 9: return new Store<25,9>(store_spec);
               case 10: return new Store<25,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<25,11>(store_spec);
               case 12: return new Store<25,12>(store_spec);
               case 13: return new Store<25,13>(store_spec);
               case 14: return new Store<25,14>(store_spec);
               case 15: return new Store<25,15>(store_spec);
#endif
             }

    case 26: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<26,1>(store_spec);
               case 2: return new Store<26,2>(store_spec);
               case 3: return new Store<26,3>(store_spec);
               case 4: return new Store<26,4>(store_spec);
               case 5: return new Store<26,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<26,6>(store_spec);
               case 7: return new Store<26,7>(store_spec);
               case 8: return new Store<26,8>(store_spec);
               case 9: return new Store<26,9>(store_spec);
               case 10: return new Store<26,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<26,11>(store_spec);
               case 12: return new Store<26,12>(store_spec);
               case 13: return new Store<26,13>(store_spec);
               case 14: return new Store<26,14>(store_spec);
               case 15: return new Store<26,15>(store_spec);
#endif
             }

    case 27: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<27,1>(store_spec);
               case 2: return new Store<27,2>(store_spec);
               case 3: return new Store<27,3>(store_spec);
               case 4: return new Store<27,4>(store_spec);
               case 5: return new Store<27,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<27,6>(store_spec);
               case 7: return new Store<27,7>(store_spec);
               case 8: return new Store<27,8>(store_spec);
               case 9: return new Store<27,9>(store_spec);
               case 10: return new Store<27,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<27,11>(store_spec);
               case 12: return new Store<27,12>(store_spec);
               case 13: return new Store<27,13>(store_spec);
               case 14: return new Store<27,14>(store_spec);
               case 15: return new Store<27,15>(store_spec);
#endif
             }

    case 28: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<28,1>(store_spec);
               case 2: return new Store<28,2>(store_spec);
               case 3: return new Store<28,3>(store_spec);
               case 4: return new Store<28,4>(store_spec);
               case 5: return new Store<28,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<28,6>(store_spec);
               case 7: return new Store<28,7>(store_spec);
               case 8: return new Store<28,8>(store_spec);
               case 9: return new Store<28,9>(store_spec);
               case 10: return new Store<28,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<28,11>(store_spec);
               case 12: return new Store<28,12>(store_spec);
               case 13: return new Store<28,13>(store_spec);
               case 14: return new Store<28,14>(store_spec);
               case 15: return new Store<28,15>(store_spec);
#endif
             }

    case 29: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<29,1>(store_spec);
               case 2: return new Store<29,2>(store_spec);
               case 3: return new Store<29,3>(store_spec);
               case 4: return new Store<29,4>(store_spec);
               case 5: return new Store<29,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<29,6>(store_spec);
               case 7: return new Store<29,7>(store_spec);
               case 8: return new Store<29,8>(store_spec);
               case 9: return new Store<29,9>(store_spec);
               case 10: return new Store<29,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<29,11>(store_spec);
               case 12: return new Store<29,12>(store_spec);
               case 13: return new Store<29,13>(store_spec);
               case 14: return new Store<29,14>(store_spec);
               case 15: return new Store<29,15>(store_spec);
#endif
             }

    case 30: switch(metrics_count) {
#if METRICS_X == 1
               case 1: return new Store<30,1>(store_spec);
               case 2: return new Store<30,2>(store_spec);
               case 3: return new Store<30,3>(store_spec);
               case 4: return new Store<30,4>(store_spec);
               case 5: return new Store<30,5>(store_spec);
#elif METRICS_X == 2
               case 6: return new Store<30,6>(store_spec);
               case 7: return new Store<30,7>(store_spec);
               case 8: return new Store<30,8>(store_spec);
               case 9: return new Store<30,9>(store_spec);
               case 10: return new Store<30,10>(store_spec);
#elif METRICS_X == 3
               case 11: return new Store<30,11>(store_spec);
               case 12: return new Store<30,12>(store_spec);
               case 13: return new Store<30,13>(store_spec);
               case 14: return new Store<30,14>(store_spec);
               case 15: return new Store<30,15>(store_spec);
#endif
             }
#endif
  }

  throw std::invalid_argument(
      "This store supports "
#if DIMS_X == 1
      "1-10"
#elif DIMS_X == 2
      "10-20"
#elif DIMS_X == 3
      "20-30"
#endif
      " dimensions and "
#if METRICS_X == 1
      "1-5"
#elif METRICS_X == 2
      "5-10"
#elif METRICS_X == 3
      "10-15"
#endif
      " metrics"
  );
}

