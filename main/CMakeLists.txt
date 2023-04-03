# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(

    SRC_DIRS  		"."  
      				"http" 
    				"wifi"
    				"1wire"
     				"sensors/ds18b20"
     				"sensors/yl-69" 
     				"measurement" 
     				"adc" 
     				"sensors/dht22"
     				"app"  				
    
    INCLUDE_DIRS    "." 
    				"http/include" 
    				"wifi/include"
    				"1wire/include"
     				"sensors/ds18b20/include"
     				"measurement/include" 
     				"sensors/yl-69/include"
     				"adc/include" 
     				"sensors/dht22/include"
     				"app/include" 
)

component_compile_options(-Wno-error=uninitialized -Wno-format)
component_compile_options(-Wno-error=unused-but-set-parameter)



