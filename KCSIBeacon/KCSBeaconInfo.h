//
//  KCSBeaconInfo.h
//  KCSIBeacon
//
//  Copyright 2014 Kinvey, Inc
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
#import <Foundation/Foundation.h>

@import CoreLocation;

@class KCSBeaconInfo;

@interface CLBeaconRegion (KCSBeaconInfo)
- (KCSBeaconInfo*) kcsBeaconInfo;
@end

// This is handy since instances change on each call
@interface KCSBeaconInfo : NSObject
@property (nonatomic, copy) NSString* uuid;
@property (nonatomic, copy) NSString* identifier;
@property (nonatomic) CLBeaconMajorValue major;
@property (nonatomic) CLBeaconMinorValue minor;
@property (nonatomic) CLLocationAccuracy accuracy;
@property (nonatomic) CLProximity proximity;
@property (nonatomic) NSInteger rssi;

- (NSDictionary*) plistObject;
- (instancetype) initWithPlistObject:(NSDictionary*)plistObject;


@end
