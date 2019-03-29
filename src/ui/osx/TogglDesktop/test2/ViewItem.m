//
//  ViewItem.m
//  Toggl Desktop on the Mac
//
//  Copyright (c) 2014 Toggl Desktop developers. All rights reserved.
//

#import "ViewItem.h"

@implementation ViewItem

- (void)load:(TogglGenericView *)data
{
	self.ID = data->ID;
	self.WID = data->WID;
	self.Name = [NSString stringWithUTF8String:data->Name];
	self.GUID = nil;
	if (data->GUID)
	{
		self.GUID = [NSString stringWithUTF8String:data->GUID];
	}
}

+ (NSArray<ViewItem *> *)loadAll:(TogglGenericView *)first
{
	NSMutableArray<ViewItem *> *result = [[NSMutableArray<ViewItem *> alloc] init];
	TogglGenericView *it = first;

	while (it)
	{
		ViewItem *item = [[ViewItem alloc] init];
		[item load:it];
		[result addObject:item];
		it = it->Next;
	}
	return [result copy];
}

@end
