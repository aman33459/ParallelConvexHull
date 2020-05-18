import subprocess
import sys
import random
import os
import platform


def shellGetOutput(str) :
  process = subprocess.Popen(str,shell=True,stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE)
  output, err = process.communicate()
  
  if (len(err) > 0):
      raise NameError(str+"\n"+output+err)
  return output
def setEnvironmentVariable(p) :
  print(os.environ)
  if os.environ.has_key("OMP_NUM_THREADS"):
    print "came inside it"
    p=6
    process = subprocess.Popen('export OMP_NUM_THREADS=5',shell=True,stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE)
    output, err = process.communicate()
    print(output)
    print(shellGetOutput('export OMP_NUM_THREADS=' + str(p)))
    print("ok")
    print p
    #os.putenv("OMP_NUM_THREADS", "%d" %p)
  else:
    shellGetOutput("export OMP_NUM_THREADS=" + str(p))  
def stripFloat(val) :
  trunc = float(int(val*1000))/1000
  return str(trunc).rstrip('0')    

def runSingle(runProgram, options, ifile, procs) :
  comString = "./"+runProgram+" "+options+" "+ifile
  if (procs > 0) :
    comString = setEnvironmentVariable(comString,procs)
  out = shellGetOutput(comString)
  #print(out)
  try:
    times = [float(str[str.index(' ')+1:]) for str in out.split('\n') if str.startswith("PBBS-time: ")]
    return times
  except (ValueError,IndexError):
    raise NameError(comString+"\n"+out)
answer=[]
def runTest(runProgram, checkProgram, dataDir, test, rounds, procs, noOutput) :
    random.seed()
    print "Output file is stores in /home/aman/Downloads/prog/convexHull/"
    outFile="/home/aman/Downloads/prog/convexHull/ofile%d_%d" %(random.randint(0, 1000000), random.randint(0, 1000000))
    [weight, inputFileNames, runOptions, checkOptions] = test
    if type(inputFileNames) is str :
      inputFileNames = [inputFileNames]
    shortInputNames = " ".join(inputFileNames)
    if len(dataDir)>0:
      out = shellGetOutput("cd " + dataDir + "; make " + shortInputNames)
    longInputNames = " ".join(dataDir + "/" + name for name in inputFileNames)
    runOptions = runOptions + " -r " + `rounds`
    if (noOutput == 0) :
      runOptions = runOptions + " -o " + outFile
    times = runSingle(runProgram, runOptions, longInputNames, procs)
    if (noOutput == 0) :
      checkString = ("./" + checkProgram + " " + checkOptions + " "
                     + longInputNames + " " + outFile)
      checkOut = shellGetOutput(checkString)
      # Allow checker output comments. Comments are lines prefixed by '::'
      nonCommentLines = [s for s in checkOut.split('\n') if not s.startswith(':') and len(s)>0]
      if (len(nonCommentLines) > 0) :
        print("CheckOut:", checkOut)
        raise NameError(checkString+"\n"+checkOut)
      os.remove(outFile)
    ptimes = str([(time)
                  for time in times])[1:-1]
    outputStr = ""
    if (len(runOptions) > 0) :
      outputStr = " : " + runOptions
    fl=0
    arm=""
    for i in shortInputNames:
      if i == '_':
        fl=1
      elif fl==1:
        arm=arm+i
    #print arm
    #raise
    print "Input file processed with time " + ptimes
    global inpfile
    nameN= "Input" + str(inpfile)+".txt" 
    inpfile=inpfile+1
    global answer
    answer.append(ptimes)
    print(nameN  + " with " + arm +" no of points is processed in : "
          + ptimes + " ")
    return [weight,times]
    
def averageTime(times) :
    return sum(times)/len(times)
    

def timeAll(name, runProgram, checkProgram, dataDir, tests, rounds, procs, noOutput,
            addToDatabase, problem) :
  totalTime = 0
  totalWeight = 0
  try:
    results = [runTest(runProgram, checkProgram, dataDir, test, rounds, procs,
                       noOutput)
               for test in tests]
    totalTimeMean = 0
    totalTimeMin = 0
    totalTimeMedian = 0
    totalWeight = 0
    j = 0
    for (weight,times) in results:
      l = len(times)
      if (l == 0):
        print("Warning, no timed results for", tests[j])
        continue
      times = sorted(times)
      #print times
      
      totalTimeMean = totalTimeMean + weight*sum(times)/l
      totalTimeMin = totalTimeMin + weight*times[0]
      totalTimeMedian = totalTimeMedian + weight*times[(l-1)/2]
      totalWeight = totalWeight + weight
      j += 1
    global answer
    sys.stdout.write("Time taken for input files are: ")
    for i in answer:
      sys.stdout.write(i + " " );
    print ""
    an1 = min(answer)
    su = 0
    for k in answer:
      su = su + float(float(k))
    an2 = su/len(answer);
    print("Therefore the parallel algorithm with " + " : " + `procs`  + 
          " processor with minimum time of =" + str(an1) + " with averageTime of = " + str(an2) + " and totalTime of =" + str(su) )
    if (addToDatabase) :
      try:
        dbAddResult(problem=problem, program=runProgram, results=results, numProcs=procs, mean=totalTimeMean/totalWeight,
                    min=totalTimeMin/totalWeight, median=totalTimeMedian/totalWeight, tests=tests)
      except:
        print("Could not insert result in database. Error:", sys.exc_info()[0])
#        if (os.getlogin() == 'akyrola'):  raise
    return 0
  except NameError,v:
    x, = v
    print "TEST TERMINATED ABNORMALLY:\n["+x + "]"
    return 1
  except KeyboardInterrupt:
    return 1


def getOption(str) :
  a = sys.argv
  l = len(a)
  for i in range(1, l) :
    if (a[i] == str) :
      return True
  return False

def getArg(str, default) :
  a = sys.argv
  l = len(a)
  for i in range(1, l) :
    if (a[i] == str and  (i+1 != l)) :
        return sys.argv[i+1]
  return default

def getArgs() :
  noOutput = getOption("-x")
  addToDatabase = getOption("-d")
  processors = int(getArg("-p", 0))
  rounds = int(getArg("-r", 1))
  return (noOutput, rounds, addToDatabase, processors)
inpfile=1 
def timeAllArgs(runProgram, problem, checkProgram, dataDir, tests) :
    (noOutput, rounds, addToDatabase, procs) = getArgs()
    name = os.path.basename(os.getcwd())
    inpfile=1
    print "--------------Parallel Convex Hull algorithm------------------------------"
    timeAll(name, runProgram, checkProgram, dataDir, tests, rounds, procs, noOutput, addToDatabase, problem)

def detectCPUModel():  
    mhz = 0
    model = platform.processor()
    try:
        if (platform.system() == "Darwin"):
            model = shellGetOutput("system_profiler SPHardwareDataType |grep 'Processor Name'")
            mhz = shellGetOutput("system_profiler SPHardwareDataType |grep 'Processor Speed'")
        else:
            model = shellGetOutput('grep "model name" /proc/cpuinfo').split('\n')[0]
            mhz = shellGetOutput('grep "cpu MHz" /proc/cpuinfo').split('\n')[0]
        model = model.split(':')[-1].strip()
        mhz = mhz.split(':')[-1].strip()
    except:
        # Could not get processor model 
        print("Could not determine CPU model", sys.exc_info()[0])
    return (model, mhz)

def detectCPUs():
    # Linux, Unix and MacOS:
    if hasattr(os, "sysconf"):
       if os.sysconf_names.has_key("SC_NPROCESSORS_ONLN"):
           # Linux & Unix:
           ncpus = os.sysconf("SC_NPROCESSORS_ONLN")
           if isinstance(ncpus, int) and ncpus > 0:
               return ncpus
       else: # OSX:
           return int(os.popen2("sysctl -n hw.ncpu")[1].read())
    # Windows:
    if os.environ.has_key("NUMBER_OF_PROCESSORS"):
           ncpus = int(os.environ["NUMBER_OF_PROCESSORS"]);
           if ncpus > 0:
               return ncpus
    return 1 # Default    

print detectCPUs()
print detectCPUModel()
setEnvironmentVariable(detectCPUs())