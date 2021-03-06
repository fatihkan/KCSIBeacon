//
//  KSBeaconManager.h
//  KCSIBeacon
//
//  Copyright 2015 Kinvey, Inc
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

@import Foundation;
@import CoreLocation;

/** The error domain for errors generated by this project (as opposed to kCLErrorDomain) */
extern NSString* const KCSIBeaconErrorDomain;

typedef NS_ENUM(NSInteger, KCSIBeaconErrorCode) {
    /** Error codes returned from a KCSIBeaconCababilityErrorDomain error */
    KCSIBeaconCannotMonitorCLBeaconRegion = -1001,
    KCSIBeaconCannotRangeIBeacons = -1002,

    KCSIBeaconInvalidUUID = -1100,
    KCSIBeaconInvalidBeaconRegion = -1101,

    KCSIBeaconLocationServicesNotEnabled = -100,
    KCSIBeaconLocationServicesDenied = -101,
    KCSIBeaconLocationServicesRestricted = -102,
};


typedef NS_ENUM(NSInteger, KCSIBeaconRegionEvent) {
    KCSIBeaconRegionEventEnter,
    KCSIBeaconRegionEventExit
};

/**
 Delegate for the `KCSBeaconManager` class. The implementing object can get notified for various beacon events.
 
 @since 1.0.0
 */
@protocol KCSBeaconManagerDelegate <NSObject>
@optional

///
/// @name Ranging
///

/** Called when error occurred during ranging.
 
 This just forwards `- CLLocationManagerDelegate locationManager:rangingBeaconsDidFailForRegion:withError:`.
 
 @param region the region that could not be ranged
 @param the error that occurred. See [xcdoc://?url=developer.apple.com/library/ios/documentation/CoreLocation/Reference/CoreLocationConstantsRef/Reference/reference.html#] for a list of the error codes and what they mean.
 
 @since 1.0.0
 */
- (void) rangingFailedForRegion:(CLBeaconRegion*)region withError:(NSError*)error;

/** Called each time a beacon is updated with new ranging information.
 
 @param beacon the beacon object
 @since 1.0.0
 */
- (void) rangedBeacon:(CLBeacon*)beacon;

/** Called when the device is closer to a a different beacon, as defined by uuid, major, and minor values.
 
 This will not be called if the new beacon is a physically different device but presents the same identification uuid and major/minor values.
 
 @param beacon the beacon object
 @see newNearestBeacon: if you just want to be notified if there is a new closest beacon
 @since 1.0.0
 */
- (void) newNearestBeacon:(CLBeacon*)beacon;

///
/// @name Region Monitoring
///

/** Called when a monitored beacon is detected 
 
 @param region the region entered
 @since 1.0.0
 */
- (void) enteredRegion:(CLBeaconRegion*)region;

/** Called when a monitored beacon is out of range.
 
 @param region the region exted
 @since 1.0.0
 */
- (void) exitedRegion:(CLBeaconRegion*)region;

/**
 *  Implement this method to override the default message that is displayed to the user when a beacon region is entered or exited. The BeaconManager's `postsLocalNotification` property should be `YES` for this method to be called.
 *
 *  @param region    the beacon region triggered
 *  @param eventCode either Enter or Exit, depending
 *
 *  @return the notification message to display to the user. `nil` if message should not be displayed
 *
 *  @since 1.1.0
 */
- (NSString*) localNotificationMessageForBeacon:(CLBeaconRegion*)region event:(KCSIBeaconRegionEvent)eventCode;

@end

/**
 A CLLocationManager wrapper to handle beacon region setup and detection.
 @since 1.0.0
 */
@interface KCSBeaconManager : NSObject

/** Set this to > 0 to ignore events within a certain time frame. This is useful if the beacons are particurally unsteady, in order to prevent confusing the user with too-frequent UI updates.
 
 Defaults to `0`.
 @since 1.0.0
 */
@property (nonatomic) NSTimeInterval monitoringInterval;

/** If set to `YES`, then UILocalNotifications will be automatically posted for entry and exit events. The `userInfo` object has two values, `"region"` which has a `KCSBeaconInfo` object with the region information and an `"event"` which is either `"enter"` or `"exit"` to describe the triggering event.
 @since 1.0.0
 */
@property (nonatomic) BOOL postsLocalNotification;

/** An optional delegate to get notified when certain events occur.
 @see KCSBeaconManagerDelegate
 @since 1.0.0
 */
@property (nonatomic, weak) id<KCSBeaconManagerDelegate> delegate;

/** Starting monitoring for a particular beacon.
 
 This monitors all beacons of a specified identifier.
 
 @param UUIDString the UUID of the beacons to monitor
 @param identifier a string to identify the region in callbacks. Particuarlly useful if there are multiple regions monitored.
 @param error set to a value if the monitoring was unable to start. These are different than errors returned after monitoring is requested. For example, if the beacon info was bad (not a valid UUID), or if the user has not authorized location services or bluetooth.
 @return YES if monitoring was able to start, NO in the cases when there is a priori knowledge that the monitoring would not work. Check the error object.
 @see startMonitoringForRegion:identifier:major:minor:
 @since 1.0.0
*/
- (BOOL) startMonitoringForRegion:(NSString*)UUIDString identifier:(NSString*)identifier error:(NSError**)error;

/** Starting monitoring for a particular beacon.
 
 This monitors all beacons of a specified identifier, with major and minor values specified.
 
 @param UUIDString the UUID of the beacons to monitor
 @param identifier a string to identify the region in callbacks. Particuarlly useful if there are multiple regions monitored.
 @param major the major value to monitor for. Can be `nil` to monitor all beacons of a particular UUID. If this is nil, `minor` must be nil too.
 @param minor the minor value to monitor for. If `nil`, this will look for all beacons of the specified UUID and major value. 
 @param error set to a value if the monitoring was unable to start. These are different than errors returned after monitoring is requested. For example, if the beacon info was bad (not a valid UUID), or if the user has not authorized location services or bluetooth.
 @return YES if monitoring was able to start, NO in the cases when there is a priori knowledge that the monitoring would not work. Check the error object.
 @see startMonitoringForRegion:identifier:
 @since 1.0.0
*/
- (BOOL) startMonitoringForRegion:(NSString*)UUIDString identifier:(NSString*)identifier major:(NSNumber*)major minor:(NSNumber*)minor error:(NSError**)error;

/** Stops ranging and monitoring a region.
 
 @param identifier the string to identify the region to stop monitoring.
 @return YES if a the region exists and was attempted to stop.
 @since 1.2.0
 */
- (BOOL) stopMonitoringForRegion:(NSString*)identifier;


@end
