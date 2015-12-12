## Here we'll generate a few test cases and print them out
## in this form: test("1 + 5 - 6", 0);
## In this way I'll have a way to generate tests and check for mistakes
## or miscalculations
## The tests will also be separated, I will write a C-program for testing
## and a separate one for interactive usage (e.g. command-line calculator)

class TestGen

  attr_accessor :size, :brack, :type, :prev
  ## there are several types: int, float, intf, random
  def initialize(type="int", size="random", brackets=false)
    if (size == "random")
      @size = (rand * 20).to_i
    else
      @size = size.to_i
    end
	
	@brack = brackets
    
	@type = type
    @ops = %w{- + * / ^ %}
#    generate()
#	p @result = eval(@expr) 
  end

  def br
    return true if rand * 10 > 6
	return false
  end

  def generate
    ## use an array instead of string, so that if the last element is
	## an arithmetic operator, I don't have to worry about whitespace
    str = []
	stack = 0
    @size.times do |i|
      choose = rand * 2
      if i % 2 == 0
	    r = (rand * 200).to_i
        if @brack && br() && stack < @size/4 && i < 18
		  t = "(" + r.to_s
		  str << t
		  stack += 1
		elsif stack >= @size/4
		  t = r.to_s + ")"
		  str << t
        else
          str << r.to_s
        end
        #str << " "
      else
        str << @ops[(rand * 5).to_i]
		#str << " "
      end
    end
#	puts "STACK: #{stack}"
	str.pop() if @ops.include?(str[-1])
    @expr = str.join(" ")
    @result = eval(str.join(" ").gsub("^", "**"))
	return str.join(" ")
  end

  def show_tests(title, times)
    out = "puts(\"" + ("=" * 80) + "\");\n"
    out += "puts(\"#{title}\");\n"
    out += "puts(\"" + ("=" * 80) + "\");\n"
    times.times {|i| generate(); out +=  "test(\"#{@expr}\", #{@result});\n"}
	@prev = out
	puts out
  end
end

t = TestGen.new("int", 20, false)
t.show_tests("Simple expression, no brackets, random numbers and operators:", 15)
