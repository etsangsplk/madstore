#ifndef MAD_STORE_FACTORY_H
#define MAD_STORE_FACTORY_H

#include <stdexcept>
#include "store.h"

using namespace std;

template<typename MetricType>
struct StoreFactory {

  static BaseStore<MetricType>* Create(StoreSpec& store_spec) {

    size_t dims_count = store_spec.DimsCount();
    size_t metrics_count = store_spec.MetricsCount();

    switch(dims_count) {
#if !defined(DIMS_2X) && !defined(DIMS_3X)
      case 1: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<1,MetricType,1>(store_spec);
                case 2: return new Store<1,MetricType,2>(store_spec);
                case 3: return new Store<1,MetricType,3>(store_spec);
                case 4: return new Store<1,MetricType,4>(store_spec);
                case 5: return new Store<1,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<1,MetricType,6>(store_spec);
                case 7: return new Store<1,MetricType,7>(store_spec);
                case 8: return new Store<1,MetricType,8>(store_spec);
                case 9: return new Store<1,MetricType,9>(store_spec);
                case 10: return new Store<1,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<1,MetricType,11>(store_spec);
                case 12: return new Store<1,MetricType,12>(store_spec);
                case 13: return new Store<1,MetricType,13>(store_spec);
                case 14: return new Store<1,MetricType,14>(store_spec);
                case 15: return new Store<1,MetricType,15>(store_spec);
#endif
              }

      case 2: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<2,MetricType,1>(store_spec);
                case 2: return new Store<2,MetricType,2>(store_spec);
                case 3: return new Store<2,MetricType,3>(store_spec);
                case 4: return new Store<2,MetricType,4>(store_spec);
                case 5: return new Store<2,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<2,MetricType,6>(store_spec);
                case 7: return new Store<2,MetricType,7>(store_spec);
                case 8: return new Store<2,MetricType,8>(store_spec);
                case 9: return new Store<2,MetricType,9>(store_spec);
                case 10: return new Store<2,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<2,MetricType,11>(store_spec);
                case 12: return new Store<2,MetricType,12>(store_spec);
                case 13: return new Store<2,MetricType,13>(store_spec);
                case 14: return new Store<2,MetricType,14>(store_spec);
                case 15: return new Store<2,MetricType,15>(store_spec);
#endif
              }

      case 3: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<3,MetricType,1>(store_spec);
                case 2: return new Store<3,MetricType,2>(store_spec);
                case 3: return new Store<3,MetricType,3>(store_spec);
                case 4: return new Store<3,MetricType,4>(store_spec);
                case 5: return new Store<3,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<3,MetricType,6>(store_spec);
                case 7: return new Store<3,MetricType,7>(store_spec);
                case 8: return new Store<3,MetricType,8>(store_spec);
                case 9: return new Store<3,MetricType,9>(store_spec);
                case 10: return new Store<3,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<3,MetricType,11>(store_spec);
                case 12: return new Store<3,MetricType,12>(store_spec);
                case 13: return new Store<3,MetricType,13>(store_spec);
                case 14: return new Store<3,MetricType,14>(store_spec);
                case 15: return new Store<3,MetricType,15>(store_spec);
#endif
              }

      case 4: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<4,MetricType,1>(store_spec);
                case 2: return new Store<4,MetricType,2>(store_spec);
                case 3: return new Store<4,MetricType,3>(store_spec);
                case 4: return new Store<4,MetricType,4>(store_spec);
                case 5: return new Store<4,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<4,MetricType,6>(store_spec);
                case 7: return new Store<4,MetricType,7>(store_spec);
                case 8: return new Store<4,MetricType,8>(store_spec);
                case 9: return new Store<4,MetricType,9>(store_spec);
                case 10: return new Store<4,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<4,MetricType,11>(store_spec);
                case 12: return new Store<4,MetricType,12>(store_spec);
                case 13: return new Store<4,MetricType,13>(store_spec);
                case 14: return new Store<4,MetricType,14>(store_spec);
                case 15: return new Store<4,MetricType,15>(store_spec);
#endif
              }

      case 5: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<5,MetricType,1>(store_spec);
                case 2: return new Store<5,MetricType,2>(store_spec);
                case 3: return new Store<5,MetricType,3>(store_spec);
                case 4: return new Store<5,MetricType,4>(store_spec);
                case 5: return new Store<5,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<5,MetricType,6>(store_spec);
                case 7: return new Store<5,MetricType,7>(store_spec);
                case 8: return new Store<5,MetricType,8>(store_spec);
                case 9: return new Store<5,MetricType,9>(store_spec);
                case 10: return new Store<5,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<5,MetricType,11>(store_spec);
                case 12: return new Store<5,MetricType,12>(store_spec);
                case 13: return new Store<5,MetricType,13>(store_spec);
                case 14: return new Store<5,MetricType,14>(store_spec);
                case 15: return new Store<5,MetricType,15>(store_spec);
#endif
              }

      case 6: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<6,MetricType,1>(store_spec);
                case 2: return new Store<6,MetricType,2>(store_spec);
                case 3: return new Store<6,MetricType,3>(store_spec);
                case 4: return new Store<6,MetricType,4>(store_spec);
                case 5: return new Store<6,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<6,MetricType,6>(store_spec);
                case 7: return new Store<6,MetricType,7>(store_spec);
                case 8: return new Store<6,MetricType,8>(store_spec);
                case 9: return new Store<6,MetricType,9>(store_spec);
                case 10: return new Store<6,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<6,MetricType,11>(store_spec);
                case 12: return new Store<6,MetricType,12>(store_spec);
                case 13: return new Store<6,MetricType,13>(store_spec);
                case 14: return new Store<6,MetricType,14>(store_spec);
                case 15: return new Store<6,MetricType,15>(store_spec);
#endif
              }

      case 7: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<7,MetricType,1>(store_spec);
                case 2: return new Store<7,MetricType,2>(store_spec);
                case 3: return new Store<7,MetricType,3>(store_spec);
                case 4: return new Store<7,MetricType,4>(store_spec);
                case 5: return new Store<7,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<7,MetricType,6>(store_spec);
                case 7: return new Store<7,MetricType,7>(store_spec);
                case 8: return new Store<7,MetricType,8>(store_spec);
                case 9: return new Store<7,MetricType,9>(store_spec);
                case 10: return new Store<7,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<7,MetricType,11>(store_spec);
                case 12: return new Store<7,MetricType,12>(store_spec);
                case 13: return new Store<7,MetricType,13>(store_spec);
                case 14: return new Store<7,MetricType,14>(store_spec);
                case 15: return new Store<7,MetricType,15>(store_spec);
#endif
              }

      case 8: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<8,MetricType,1>(store_spec);
                case 2: return new Store<8,MetricType,2>(store_spec);
                case 3: return new Store<8,MetricType,3>(store_spec);
                case 4: return new Store<8,MetricType,4>(store_spec);
                case 5: return new Store<8,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<8,MetricType,6>(store_spec);
                case 7: return new Store<8,MetricType,7>(store_spec);
                case 8: return new Store<8,MetricType,8>(store_spec);
                case 9: return new Store<8,MetricType,9>(store_spec);
                case 10: return new Store<8,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<8,MetricType,11>(store_spec);
                case 12: return new Store<8,MetricType,12>(store_spec);
                case 13: return new Store<8,MetricType,13>(store_spec);
                case 14: return new Store<8,MetricType,14>(store_spec);
                case 15: return new Store<8,MetricType,15>(store_spec);
#endif
              }

      case 9: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                case 1: return new Store<9,MetricType,1>(store_spec);
                case 2: return new Store<9,MetricType,2>(store_spec);
                case 3: return new Store<9,MetricType,3>(store_spec);
                case 4: return new Store<9,MetricType,4>(store_spec);
                case 5: return new Store<9,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                case 6: return new Store<9,MetricType,6>(store_spec);
                case 7: return new Store<9,MetricType,7>(store_spec);
                case 8: return new Store<9,MetricType,8>(store_spec);
                case 9: return new Store<9,MetricType,9>(store_spec);
                case 10: return new Store<9,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                case 11: return new Store<9,MetricType,11>(store_spec);
                case 12: return new Store<9,MetricType,12>(store_spec);
                case 13: return new Store<9,MetricType,13>(store_spec);
                case 14: return new Store<9,MetricType,14>(store_spec);
                case 15: return new Store<9,MetricType,15>(store_spec);
#endif
              }

      case 10: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<10,MetricType,1>(store_spec);
                 case 2: return new Store<10,MetricType,2>(store_spec);
                 case 3: return new Store<10,MetricType,3>(store_spec);
                 case 4: return new Store<10,MetricType,4>(store_spec);
                 case 5: return new Store<10,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<10,MetricType,6>(store_spec);
                 case 7: return new Store<10,MetricType,7>(store_spec);
                 case 8: return new Store<10,MetricType,8>(store_spec);
                 case 9: return new Store<10,MetricType,9>(store_spec);
                 case 10: return new Store<10,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<10,MetricType,11>(store_spec);
                 case 12: return new Store<10,MetricType,12>(store_spec);
                 case 13: return new Store<10,MetricType,13>(store_spec);
                 case 14: return new Store<10,MetricType,14>(store_spec);
                 case 15: return new Store<10,MetricType,15>(store_spec);
#endif
               }

#elif defined(DIMS_2X)
      case 11: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<11,MetricType,1>(store_spec);
                 case 2: return new Store<11,MetricType,2>(store_spec);
                 case 3: return new Store<11,MetricType,3>(store_spec);
                 case 4: return new Store<11,MetricType,4>(store_spec);
                 case 5: return new Store<11,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<11,MetricType,6>(store_spec);
                 case 7: return new Store<11,MetricType,7>(store_spec);
                 case 8: return new Store<11,MetricType,8>(store_spec);
                 case 9: return new Store<11,MetricType,9>(store_spec);
                 case 10: return new Store<11,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<11,MetricType,11>(store_spec);
                 case 12: return new Store<11,MetricType,12>(store_spec);
                 case 13: return new Store<11,MetricType,13>(store_spec);
                 case 14: return new Store<11,MetricType,14>(store_spec);
                 case 15: return new Store<11,MetricType,15>(store_spec);
#endif
               }

      case 12: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<12,MetricType,1>(store_spec);
                 case 2: return new Store<12,MetricType,2>(store_spec);
                 case 3: return new Store<12,MetricType,3>(store_spec);
                 case 4: return new Store<12,MetricType,4>(store_spec);
                 case 5: return new Store<12,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<12,MetricType,6>(store_spec);
                 case 7: return new Store<12,MetricType,7>(store_spec);
                 case 8: return new Store<12,MetricType,8>(store_spec);
                 case 9: return new Store<12,MetricType,9>(store_spec);
                 case 10: return new Store<12,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<12,MetricType,11>(store_spec);
                 case 12: return new Store<12,MetricType,12>(store_spec);
                 case 13: return new Store<12,MetricType,13>(store_spec);
                 case 14: return new Store<12,MetricType,14>(store_spec);
                 case 15: return new Store<12,MetricType,15>(store_spec);
#endif
               }

      case 13: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<13,MetricType,1>(store_spec);
                 case 2: return new Store<13,MetricType,2>(store_spec);
                 case 3: return new Store<13,MetricType,3>(store_spec);
                 case 4: return new Store<13,MetricType,4>(store_spec);
                 case 5: return new Store<13,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<13,MetricType,6>(store_spec);
                 case 7: return new Store<13,MetricType,7>(store_spec);
                 case 8: return new Store<13,MetricType,8>(store_spec);
                 case 9: return new Store<13,MetricType,9>(store_spec);
                 case 10: return new Store<13,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<13,MetricType,11>(store_spec);
                 case 12: return new Store<13,MetricType,12>(store_spec);
                 case 13: return new Store<13,MetricType,13>(store_spec);
                 case 14: return new Store<13,MetricType,14>(store_spec);
                 case 15: return new Store<13,MetricType,15>(store_spec);
#endif
               }

      case 14: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<14,MetricType,1>(store_spec);
                 case 2: return new Store<14,MetricType,2>(store_spec);
                 case 3: return new Store<14,MetricType,3>(store_spec);
                 case 4: return new Store<14,MetricType,4>(store_spec);
                 case 5: return new Store<14,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<14,MetricType,6>(store_spec);
                 case 7: return new Store<14,MetricType,7>(store_spec);
                 case 8: return new Store<14,MetricType,8>(store_spec);
                 case 9: return new Store<14,MetricType,9>(store_spec);
                 case 10: return new Store<14,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<14,MetricType,11>(store_spec);
                 case 12: return new Store<14,MetricType,12>(store_spec);
                 case 13: return new Store<14,MetricType,13>(store_spec);
                 case 14: return new Store<14,MetricType,14>(store_spec);
                 case 15: return new Store<14,MetricType,15>(store_spec);
#endif
               }

      case 15: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<15,MetricType,1>(store_spec);
                 case 2: return new Store<15,MetricType,2>(store_spec);
                 case 3: return new Store<15,MetricType,3>(store_spec);
                 case 4: return new Store<15,MetricType,4>(store_spec);
                 case 5: return new Store<15,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<15,MetricType,6>(store_spec);
                 case 7: return new Store<15,MetricType,7>(store_spec);
                 case 8: return new Store<15,MetricType,8>(store_spec);
                 case 9: return new Store<15,MetricType,9>(store_spec);
                 case 10: return new Store<15,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<15,MetricType,11>(store_spec);
                 case 12: return new Store<15,MetricType,12>(store_spec);
                 case 13: return new Store<15,MetricType,13>(store_spec);
                 case 14: return new Store<15,MetricType,14>(store_spec);
                 case 15: return new Store<15,MetricType,15>(store_spec);
#endif
               }

      case 16: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<16,MetricType,1>(store_spec);
                 case 2: return new Store<16,MetricType,2>(store_spec);
                 case 3: return new Store<16,MetricType,3>(store_spec);
                 case 4: return new Store<16,MetricType,4>(store_spec);
                 case 5: return new Store<16,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<16,MetricType,6>(store_spec);
                 case 7: return new Store<16,MetricType,7>(store_spec);
                 case 8: return new Store<16,MetricType,8>(store_spec);
                 case 9: return new Store<16,MetricType,9>(store_spec);
                 case 10: return new Store<16,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<16,MetricType,11>(store_spec);
                 case 12: return new Store<16,MetricType,12>(store_spec);
                 case 13: return new Store<16,MetricType,13>(store_spec);
                 case 14: return new Store<16,MetricType,14>(store_spec);
                 case 15: return new Store<16,MetricType,15>(store_spec);
#endif
               }

      case 17: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<17,MetricType,1>(store_spec);
                 case 2: return new Store<17,MetricType,2>(store_spec);
                 case 3: return new Store<17,MetricType,3>(store_spec);
                 case 4: return new Store<17,MetricType,4>(store_spec);
                 case 5: return new Store<17,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<17,MetricType,6>(store_spec);
                 case 7: return new Store<17,MetricType,7>(store_spec);
                 case 8: return new Store<17,MetricType,8>(store_spec);
                 case 9: return new Store<17,MetricType,9>(store_spec);
                 case 10: return new Store<17,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<17,MetricType,11>(store_spec);
                 case 12: return new Store<17,MetricType,12>(store_spec);
                 case 13: return new Store<17,MetricType,13>(store_spec);
                 case 14: return new Store<17,MetricType,14>(store_spec);
                 case 15: return new Store<17,MetricType,15>(store_spec);
#endif
               }

      case 18: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<18,MetricType,1>(store_spec);
                 case 2: return new Store<18,MetricType,2>(store_spec);
                 case 3: return new Store<18,MetricType,3>(store_spec);
                 case 4: return new Store<18,MetricType,4>(store_spec);
                 case 5: return new Store<18,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<18,MetricType,6>(store_spec);
                 case 7: return new Store<18,MetricType,7>(store_spec);
                 case 8: return new Store<18,MetricType,8>(store_spec);
                 case 9: return new Store<18,MetricType,9>(store_spec);
                 case 10: return new Store<18,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<18,MetricType,11>(store_spec);
                 case 12: return new Store<18,MetricType,12>(store_spec);
                 case 13: return new Store<18,MetricType,13>(store_spec);
                 case 14: return new Store<18,MetricType,14>(store_spec);
                 case 15: return new Store<18,MetricType,15>(store_spec);
#endif
               }

      case 19: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<19,MetricType,1>(store_spec);
                 case 2: return new Store<19,MetricType,2>(store_spec);
                 case 3: return new Store<19,MetricType,3>(store_spec);
                 case 4: return new Store<19,MetricType,4>(store_spec);
                 case 5: return new Store<19,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<19,MetricType,6>(store_spec);
                 case 7: return new Store<19,MetricType,7>(store_spec);
                 case 8: return new Store<19,MetricType,8>(store_spec);
                 case 9: return new Store<19,MetricType,9>(store_spec);
                 case 10: return new Store<19,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<19,MetricType,11>(store_spec);
                 case 12: return new Store<19,MetricType,12>(store_spec);
                 case 13: return new Store<19,MetricType,13>(store_spec);
                 case 14: return new Store<19,MetricType,14>(store_spec);
                 case 15: return new Store<19,MetricType,15>(store_spec);
#endif
               }

#elif defined(DIMS_3X)
      case 20: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<20,MetricType,1>(store_spec);
                 case 2: return new Store<20,MetricType,2>(store_spec);
                 case 3: return new Store<20,MetricType,3>(store_spec);
                 case 4: return new Store<20,MetricType,4>(store_spec);
                 case 5: return new Store<20,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<20,MetricType,6>(store_spec);
                 case 7: return new Store<20,MetricType,7>(store_spec);
                 case 8: return new Store<20,MetricType,8>(store_spec);
                 case 9: return new Store<20,MetricType,9>(store_spec);
                 case 10: return new Store<20,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<20,MetricType,11>(store_spec);
                 case 12: return new Store<20,MetricType,12>(store_spec);
                 case 13: return new Store<20,MetricType,13>(store_spec);
                 case 14: return new Store<20,MetricType,14>(store_spec);
                 case 15: return new Store<20,MetricType,15>(store_spec);
#endif
               }

      case 21: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<21,MetricType,1>(store_spec);
                 case 2: return new Store<21,MetricType,2>(store_spec);
                 case 3: return new Store<21,MetricType,3>(store_spec);
                 case 4: return new Store<21,MetricType,4>(store_spec);
                 case 5: return new Store<21,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<21,MetricType,6>(store_spec);
                 case 7: return new Store<21,MetricType,7>(store_spec);
                 case 8: return new Store<21,MetricType,8>(store_spec);
                 case 9: return new Store<21,MetricType,9>(store_spec);
                 case 10: return new Store<21,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<21,MetricType,11>(store_spec);
                 case 12: return new Store<21,MetricType,12>(store_spec);
                 case 13: return new Store<21,MetricType,13>(store_spec);
                 case 14: return new Store<21,MetricType,14>(store_spec);
                 case 15: return new Store<21,MetricType,15>(store_spec);
#endif
               }

      case 22: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<22,MetricType,1>(store_spec);
                 case 2: return new Store<22,MetricType,2>(store_spec);
                 case 3: return new Store<22,MetricType,3>(store_spec);
                 case 4: return new Store<22,MetricType,4>(store_spec);
                 case 5: return new Store<22,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<22,MetricType,6>(store_spec);
                 case 7: return new Store<22,MetricType,7>(store_spec);
                 case 8: return new Store<22,MetricType,8>(store_spec);
                 case 9: return new Store<22,MetricType,9>(store_spec);
                 case 10: return new Store<22,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<22,MetricType,11>(store_spec);
                 case 12: return new Store<22,MetricType,12>(store_spec);
                 case 13: return new Store<22,MetricType,13>(store_spec);
                 case 14: return new Store<22,MetricType,14>(store_spec);
                 case 15: return new Store<22,MetricType,15>(store_spec);
#endif
               }

      case 23: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<23,MetricType,1>(store_spec);
                 case 2: return new Store<23,MetricType,2>(store_spec);
                 case 3: return new Store<23,MetricType,3>(store_spec);
                 case 4: return new Store<23,MetricType,4>(store_spec);
                 case 5: return new Store<23,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<23,MetricType,6>(store_spec);
                 case 7: return new Store<23,MetricType,7>(store_spec);
                 case 8: return new Store<23,MetricType,8>(store_spec);
                 case 9: return new Store<23,MetricType,9>(store_spec);
                 case 10: return new Store<23,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<23,MetricType,11>(store_spec);
                 case 12: return new Store<23,MetricType,12>(store_spec);
                 case 13: return new Store<23,MetricType,13>(store_spec);
                 case 14: return new Store<23,MetricType,14>(store_spec);
                 case 15: return new Store<23,MetricType,15>(store_spec);
#endif
               }

      case 24: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<24,MetricType,1>(store_spec);
                 case 2: return new Store<24,MetricType,2>(store_spec);
                 case 3: return new Store<24,MetricType,3>(store_spec);
                 case 4: return new Store<24,MetricType,4>(store_spec);
                 case 5: return new Store<24,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<24,MetricType,6>(store_spec);
                 case 7: return new Store<24,MetricType,7>(store_spec);
                 case 8: return new Store<24,MetricType,8>(store_spec);
                 case 9: return new Store<24,MetricType,9>(store_spec);
                 case 10: return new Store<24,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<24,MetricType,11>(store_spec);
                 case 12: return new Store<24,MetricType,12>(store_spec);
                 case 13: return new Store<24,MetricType,13>(store_spec);
                 case 14: return new Store<24,MetricType,14>(store_spec);
                 case 15: return new Store<24,MetricType,15>(store_spec);
#endif
               }

      case 25: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<25,MetricType,1>(store_spec);
                 case 2: return new Store<25,MetricType,2>(store_spec);
                 case 3: return new Store<25,MetricType,3>(store_spec);
                 case 4: return new Store<25,MetricType,4>(store_spec);
                 case 5: return new Store<25,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<25,MetricType,6>(store_spec);
                 case 7: return new Store<25,MetricType,7>(store_spec);
                 case 8: return new Store<25,MetricType,8>(store_spec);
                 case 9: return new Store<25,MetricType,9>(store_spec);
                 case 10: return new Store<25,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<25,MetricType,11>(store_spec);
                 case 12: return new Store<25,MetricType,12>(store_spec);
                 case 13: return new Store<25,MetricType,13>(store_spec);
                 case 14: return new Store<25,MetricType,14>(store_spec);
                 case 15: return new Store<25,MetricType,15>(store_spec);
#endif
               }

      case 26: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<26,MetricType,1>(store_spec);
                 case 2: return new Store<26,MetricType,2>(store_spec);
                 case 3: return new Store<26,MetricType,3>(store_spec);
                 case 4: return new Store<26,MetricType,4>(store_spec);
                 case 5: return new Store<26,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<26,MetricType,6>(store_spec);
                 case 7: return new Store<26,MetricType,7>(store_spec);
                 case 8: return new Store<26,MetricType,8>(store_spec);
                 case 9: return new Store<26,MetricType,9>(store_spec);
                 case 10: return new Store<26,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<26,MetricType,11>(store_spec);
                 case 12: return new Store<26,MetricType,12>(store_spec);
                 case 13: return new Store<26,MetricType,13>(store_spec);
                 case 14: return new Store<26,MetricType,14>(store_spec);
                 case 15: return new Store<26,MetricType,15>(store_spec);
#endif
               }

      case 27: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<27,MetricType,1>(store_spec);
                 case 2: return new Store<27,MetricType,2>(store_spec);
                 case 3: return new Store<27,MetricType,3>(store_spec);
                 case 4: return new Store<27,MetricType,4>(store_spec);
                 case 5: return new Store<27,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<27,MetricType,6>(store_spec);
                 case 7: return new Store<27,MetricType,7>(store_spec);
                 case 8: return new Store<27,MetricType,8>(store_spec);
                 case 9: return new Store<27,MetricType,9>(store_spec);
                 case 10: return new Store<27,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<27,MetricType,11>(store_spec);
                 case 12: return new Store<27,MetricType,12>(store_spec);
                 case 13: return new Store<27,MetricType,13>(store_spec);
                 case 14: return new Store<27,MetricType,14>(store_spec);
                 case 15: return new Store<27,MetricType,15>(store_spec);
#endif
               }

      case 28: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<28,MetricType,1>(store_spec);
                 case 2: return new Store<28,MetricType,2>(store_spec);
                 case 3: return new Store<28,MetricType,3>(store_spec);
                 case 4: return new Store<28,MetricType,4>(store_spec);
                 case 5: return new Store<28,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<28,MetricType,6>(store_spec);
                 case 7: return new Store<28,MetricType,7>(store_spec);
                 case 8: return new Store<28,MetricType,8>(store_spec);
                 case 9: return new Store<28,MetricType,9>(store_spec);
                 case 10: return new Store<28,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<28,MetricType,11>(store_spec);
                 case 12: return new Store<28,MetricType,12>(store_spec);
                 case 13: return new Store<28,MetricType,13>(store_spec);
                 case 14: return new Store<28,MetricType,14>(store_spec);
                 case 15: return new Store<28,MetricType,15>(store_spec);
#endif
               }

      case 29: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<29,MetricType,1>(store_spec);
                 case 2: return new Store<29,MetricType,2>(store_spec);
                 case 3: return new Store<29,MetricType,3>(store_spec);
                 case 4: return new Store<29,MetricType,4>(store_spec);
                 case 5: return new Store<29,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<29,MetricType,6>(store_spec);
                 case 7: return new Store<29,MetricType,7>(store_spec);
                 case 8: return new Store<29,MetricType,8>(store_spec);
                 case 9: return new Store<29,MetricType,9>(store_spec);
                 case 10: return new Store<29,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<29,MetricType,11>(store_spec);
                 case 12: return new Store<29,MetricType,12>(store_spec);
                 case 13: return new Store<29,MetricType,13>(store_spec);
                 case 14: return new Store<29,MetricType,14>(store_spec);
                 case 15: return new Store<29,MetricType,15>(store_spec);
#endif
               }

      case 30: switch(metrics_count) {
#if !defined(METRICS_2X) && !defined(METRICS_3X)
                 case 1: return new Store<30,MetricType,1>(store_spec);
                 case 2: return new Store<30,MetricType,2>(store_spec);
                 case 3: return new Store<30,MetricType,3>(store_spec);
                 case 4: return new Store<30,MetricType,4>(store_spec);
                 case 5: return new Store<30,MetricType,5>(store_spec);
#elif defined(METRICS_2X)
                 case 6: return new Store<30,MetricType,6>(store_spec);
                 case 7: return new Store<30,MetricType,7>(store_spec);
                 case 8: return new Store<30,MetricType,8>(store_spec);
                 case 9: return new Store<30,MetricType,9>(store_spec);
                 case 10: return new Store<30,MetricType,10>(store_spec);
#elif defined(METRICS_3X)
                 case 11: return new Store<30,MetricType,11>(store_spec);
                 case 12: return new Store<30,MetricType,12>(store_spec);
                 case 13: return new Store<30,MetricType,13>(store_spec);
                 case 14: return new Store<30,MetricType,14>(store_spec);
                 case 15: return new Store<30,MetricType,15>(store_spec);
#endif
               }
#endif
    }

    throw invalid_argument(
        "This store supports "
#if !defined(DIMS_2X) && !defined(DIMS_3X)
        "1-10"
#elif defined(DIMS_2X)
        "10-20"
#elif defined(DIMS_3X)
        "20-30"
#endif
        " dimensions and "
#if !defined(METRICS_2X) && !defined(METRICS_3X)
        "1-5"
#elif defined(METRICS_2X)
        "5-10"
#elif defined(METRICS_3X)
        "10-15"
#endif
        " metrics"
    );
  }
};

#endif /* MAD_STORE_FACTORY_H */
