
# Advanced Tutorial 
Just three steps.  

## 1, Given a enum definition file.  
```C++
// Color.txt
enum class Color:int{
    Red,
    Green,
    Blue
};
```

## 2, Given a sample file.  
Infact it is a code file, but you can insert [**cec enum tags**](#TheTagTable) into the code file as a sample.
```C++
// sample.txt
struct {cec:enum:name}
{
    {cec:enum:keyValueList}

    {cec:enum:type} min(){ return {cec:enum:min}; }
    {cec:enum:type} max(){ return {cec:enum:max}; }
};
```
> Don't worry, you just have to imagine that the [**cec enum tags**](#TheTagTable) provided by cec represents the ultimate abstraction, it can be used in the sample file here, it will tell you everything about the enum and reflect it back to reality.  

## 3, Generate code.
Just run the command.
```
cec Color.txt --import sample.txt --output Color.h
```
This is the generated **Color.h** file.   
```C++
// Color.h
struct Color
{
    static const int Red = 0;
    static const int Green = 1;
    static const int Blue = 2;

    int min(){ return 0; }
    int max(){ return 2; }
};
```
So a basic reflection enum is done, and you can learn more from the cec internal samples.  
- The sample used by cec for enum code generation is [here](../sample/CodeSample.txt).  
- The sample used by cec for the enum code's **unit test code** generation is [here](../sample/CodeUnitTestSample.txt).  

# <a id="TheTagTable">The cec enum tags.</a>
All the tags definition is here, I will use the step1's color.txt for explaination.

| NO |      Tag              | Explaination                     |
|:---|:----------------------|:---------------------------------|
| 1  |{cec:enum:name}        | Color |
| 2  |{cec:enum:fullName}    | enum class Color |
| 3  |{cec:enum:type}        | int |
| 4  |{cec:enum:min}         | 0 |
| 5  |{cec:enum:max}         | 2 |
| 6  |{cec:enum:size}        | 3 |
| 7  |{cec:enum:firstKey}    | Red |
| 8  |{cec:enum:lastKey}     | Blue |
| 9  |{cec:enum:keyList}     | A list of keys |
| 10 |{cec:enum:keyValueList}| A list of key=value pairs, see the generated code in step3. |
| 11 |{cec:keep:comment}     | It will keep source file's comment when generate code. |

## About {cec:keep:comment}
For example if your have a source file definition with comment like this, when generate code, all comment will be kept in the final generated code file. A usage of this tag is [here](../sample/CodeSample.txt).  
```
/* Source with comment */
enum class Color:int{
    Red,        ///< comment line 1;
    Green,      ///< comment line 2;
    Blue        ///< comment line 3;
};
```

[Go back to parent.](../readme.md#get-start-here)
