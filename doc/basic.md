# <a id="BasicTutorial">Basic tutorial</a>
Just two steps.

## 1, Define your enum.

Define your enum and wirte it to a source file([like this](../test/Color.txt)).  
The definition of the enum should satisfy follow grammar.  
![](./imgs/enum-class-def-custom.png)
- **name:** Specify your **enum name** (aka **enum class name**).
- **type:** Specify the enum **internal value type** (aka **scalar type**).
  - It should be one word. For example, if you want to use **unsigned char**, use **uint8_t** instead.
  - Custom type such as **foo_t** or **bar_t** is ok, but the value range won't be checked.
  - The default **type** is **int**.
- **key:** Specify your enum key(s).
- **number:** Specify a decimal number or a hexadecimal number.
  - **Only the first key** can be assigned with a number(which can be negative).
  - Other keys will be assigned one by increase the number accrodingly.
  - The default **number** is **0**.

## 2, Generate code.

Run command: "**cec \<file1> --output \<file2>**" to compile the source file1 and generate code file2.  

That's all.  

[Go back to parent.](../readme.md#get-start-here)


