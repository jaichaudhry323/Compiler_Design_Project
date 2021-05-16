//jshint esversion:6
// http://192.168.29.106:3000
const express = require('express');
const cors = require('cors')
const app = express();
const http = require('http').Server(app);
const port = process.env.PORT || 3000;

const util = require('util');
const fs = require('fs')

const bodyParser = require('body-parser');
app.use(cors());
app.use(bodyParser.json());

app.use('/', (req, res) => {

  // Compiler Execution
  // res.send("{code_output:{}, error:0 or 1}");
  console.log("received request")
  console.log(req.query)  
  console.log(req.query['source_code'])


  var source_code = req.query['source_code'];
  var languageId = req.query['languageId'];
  var stdin = req.query['stdin'];

  const {exec} = require('child_process');

  function Executor() {

    if(languageId=='0')
    {
      // Mini - Compiler 

       console.log("writing to input file");
      fs.writeFile('input.txt', source_code, (err) => {
        if (err) { throw err };
      })

       var child = exec(
      './impl < input.txt > output.txt && ./impl3 < input.txt > output3.txt',
      function()
      {
        // console.log('stdout:', stdout);
        // console.error('stderr:', stderr);
        console.log("compiled");
        console.log("reading output");

        fs.readFile('output.txt', 'utf-8', function (err, data) {
          if (err) throw err;
          console.log("OUTPUT: ", data)

           fs.readFile('output3.txt', 'utf-8', function (err, data2) {
            if (err) throw err;

            console.log("OUTPUT: ", data2)

            res.json(
                    [ 
                      { 
                        "data":data,
                        "three_address_code":data2
                      }
                    ]
                );
                  console.log("Sent");
          });

        });

      }
      );
    }

// ./input.exe <input1.txt> output1.txt
// g++ input.cpp -o input.exe

    else
    {
      // Normal C++ Compiler

       console.log("writing to input file");
      fs.writeFile('input.cpp', source_code, (err) => {
        if (err) { throw err };
        // res.json({
        //   "error":"error writing source code to file"
        // })
      })

      fs.writeFile('input1.txt', stdin, (err) => {
        if (err) { throw err };
        // res.json({
        //   "error":"error writing source code to file"
        // })
      })

      var child = exec(
        ' g++ input.cpp -o input.exe && ./input.exe <input1.txt> output1.txt '
        ,
        function (error, stdout, stderr) 
        {
          
          console.log("error:",error);
          console.log('stdout:', stdout);
          console.error('stderr:', stderr);

          if(stderr)
          {
            res.json(
                  [ 
                    {
                      "data":stderr
                    }
                  ]
              )
            return ;
          } 

        
          console.log("compiled");
          console.log("reading output");

          fs.readFile('output1.txt', 'utf-8', function (err, data) {
            if (err) throw err;
            console.log("OUTPUT: ", data)

            res.json(
                  [
                    {
                      "data":data
                    }
                  ]
              );
            console.log("Sent");

          });
        }
      );
    }   
    

  }

  console.log("executing");
  Executor();

})

// http.listen(port, '192.168.29.106', () => console.log('listening on port ' + port));
// http.listen(port, '106.210.106.146', () => console.log('listening on port ' + port));

// localhost 
http.listen(port, () => console.log('listening on port ' + port));

// TODO:
// implement or integrate the following functionality:
// 1) Scan
// 2) Array
// 3) Functions
// 4) Print String functionality
// 5) enabling the variable names to be string by using a hashmap
