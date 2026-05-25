//
//  main.m
//  ardec
//
//  Copyright (c) 2026 wry
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#import <Foundation/Foundation.h>

// MARK: - CFKeyedArchiverUID

typedef struct
{
    uint8_t padding[16];
    uint32_t value;
} AAKeyedArchiverUID;

// MARK: - ARDecoder

@interface ARDecoder : NSObject
@property (nonatomic,strong) NSDictionary* archive;
@property (nonatomic,strong) NSArray* archiveObjects;
@property (nonatomic,strong) NSMutableArray<NSString*>* source;
- (instancetype)initWithArchiveData:(NSData*)data;
- (void) generateSource;
- (NSDictionary*) plistFromBinaryPlist:(NSData*)data;
- (NSData*) binaryPlistFromPlist:(NSDictionary*)plist;
@end

@implementation ARDecoder
- (instancetype)initWithArchiveData:(NSData*)data
{
    self = [super init];
    if (self) {
        // $version, $objects, $archiver, $top
        self.archive = [self plistFromBinaryPlist:data];
        self.archiveObjects = [self.archive objectForKey:@"$objects"];
        self.source = [NSMutableArray new];
    }
    return self;
}
- (void) generatePropertyForObject:(id)object withPropertyName:(NSString*)propertyName
{
    if ([object isKindOfClass:[NSString class]])
    {
        [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) NSString* %@;", propertyName]];
    }
    else if ([object isKindOfClass:[NSNumber class]])
    {
        [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) NSNumber* %@;", propertyName]];
    }
    else if ([object isKindOfClass:[NSDate class]])
    {
        [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) NSDate* %@;", propertyName]];
    }
    else if ([object isKindOfClass:[NSData class]])
    {
        [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) NSData* %@;", propertyName]];
    }
    else if ([object isKindOfClass:[NSArray class]])
    {
        [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) NSArray* %@;", propertyName]];
    }
    else if ([object isKindOfClass:[NSDictionary class]])
    {
        [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) NSDictionary* %@;", propertyName]];
    }
    else if ([object isKindOfClass:[NSObject class]])
    {
        NSDictionary* objectPropertiesDict = [self.archiveObjects objectAtIndex:((__bridge AAKeyedArchiverUID*)(object))->value];
        
        if ([objectPropertiesDict isKindOfClass:[NSDictionary class]])
        {
            NSString* class = [objectPropertiesDict objectForKey:@"$class"];
            NSString* className = [objectPropertiesDict objectForKey:@"$classname"];
            
            if (class)
            {
                NSDictionary* classDict = [self.archiveObjects objectAtIndex:((__bridge AAKeyedArchiverUID*)(class))->value];
                NSString* className = [classDict objectForKey:@"$classname"];
                [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) %@* %@;", className, propertyName]];
            }
            else if (className)
            {
                [self.source addObject:[NSString stringWithFormat:@"@property (nonatomic,strong) %@* %@;", className, propertyName]];
            }
            else
            {
                [self.source addObject:[NSString stringWithFormat:@"// skipping property generation for %@", propertyName]];
            }
        }
        else
        {
            [self generatePropertyForObject:objectPropertiesDict withPropertyName:propertyName];
        }
    }
}
- (void) generateSource
{
    for (NSDictionary* objectDict in self.archiveObjects)
    {
        if (![objectDict isKindOfClass:[NSDictionary class]])
        {
            continue;
        }
        
        // check if object has a class definition
        id classArchiverUid = [objectDict objectForKey:@"$class"];
        if (!classArchiverUid)
        {
            continue;
        }
        
        // check if class is known
        NSDictionary* classDict = [self.archiveObjects objectAtIndex:((__bridge AAKeyedArchiverUID*)(classArchiverUid))->value];
        NSString* className = [classDict objectForKey:@"$classname"];
        
        if (!NSClassFromString(className))
        {
            [self.source addObject:[NSString stringWithFormat:@"@interface %@: NSObject", className]];
            for (NSString* propertyName in objectDict.allKeys)
            {
                if ([propertyName isEqualToString:@"$class"])
                {
                    continue;
                }
                
                id object = [objectDict objectForKey:propertyName];
                
                [self generatePropertyForObject:object withPropertyName:propertyName];
            }
            [self.source addObject:@"@end"];
            [self.source addObject:[NSString stringWithFormat:@"@implementation %@", className]];
            [self.source addObject:@"@end"];
        }
    }
}
- (NSDictionary*) plistFromBinaryPlist:(NSData*)data
{
    NSError *error = nil;
    NSPropertyListFormat format;
    
    id plist = [NSPropertyListSerialization propertyListWithData:data
                                                         options:NSPropertyListImmutable
                                                          format:&format
                                                           error:&error];
    
    if (error)
    {
        NSLog(@"error deserializing plist : %@", error);
        return nil;
    }
    
    return plist;
}

- (NSData*) binaryPlistFromPlist:(NSDictionary*)plist
{
    NSError *error = nil;
    NSPropertyListFormat format;
    
    NSData* plistData = [NSPropertyListSerialization dataWithPropertyList:plist
                                                                   format:NSPropertyListBinaryFormat_v1_0
                                                                  options:0
                                                                    error:&error];
    if (error)
    {
        NSLog(@"error serializing plist : %@", error);
        return nil;
    }
    
    return plistData;
}
@end

// MARK: - Main

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        NSLog(@"0 <path> ");
        return 1;
    }
    
    NSString* path = [NSString stringWithCString:argv[1] encoding:NSUTF8StringEncoding];
    
    NSError* error = nil;
    NSData* data = [NSData dataWithContentsOfFile:path options:0 error:&error];
    if (error)
    {
        NSLog(@"error loading data : %@", error);
        return 1;
    }
    
    ARDecoder* decoder = [[ARDecoder alloc] initWithArchiveData:data];
    [decoder generateSource];
    
    NSString* sourceCode = [decoder.source componentsJoinedByString:@"\n"];

    NSFileHandle* outfh = [NSFileHandle fileHandleWithStandardOutput];
    [outfh writeData:[sourceCode dataUsingEncoding:NSUTF8StringEncoding]];
    
    return 0;
}

