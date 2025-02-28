class queue:
    def __init__(self):
        self.stack1  = []
        self.stack2 = []

    def push(self,ele):
        self.stack1.append(ele)

    def pop(self):
        val  = self.stack1.pop()
        self.stack2.append(val)
        print(f"popped {val}")
        
q = queue()
q.push(1)
q.push(2)
q.push(3)
q.pop()
