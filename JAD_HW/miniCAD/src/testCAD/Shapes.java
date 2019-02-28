package testCAD;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.io.Serializable;

public abstract class Shapes implements Serializable{
	public Color color;
    public Point p1;
    public Point p2;
    public float fine;
    Shapes(Point p1,Point p2)
    {
        this.p1=new Point(p1.x,p1.y);
        this.p2=new Point(p2.x,p2.y);
        this.fine = 3;
        this.color = Color.BLACK;
    }
	
    public void setColor(Color c) {
    	this.color = c;
    }
    
    public void setLine(float fine , Graphics g ) {
        Graphics2D g2d=(Graphics2D) g;
        g2d.setStroke(new BasicStroke(fine));
    }
    
    Point[] rectify() 
    {
        if(p1.x>p2.x&&p1.y>p2.y)
        {
            return new Point[]{p2,p1};
        }
        else if(p2.x>p1.x&&p2.y>p1.y)
        {
            return new Point[]{p1,p2};
        }
        else if(p2.x>p1.x&&p2.y<p1.y)
        {
            return new Point[]{new Point(p1.x,p2.y),new Point(p2.x,p1.y)};
        }
        else
        {
            return new Point[]{new Point(p2.x,p1.y),new Point(p1.x,p2.y)};
        }
    }
    
    void setSize(int flag) {  //变大或变小
    	int p0_x = (p1.x+p2.x)/2;
    	int p0_y = (p1.y+p2.y)/2;
    	
    	int p1_p0_x = p1.x-p0_x;
    	int p1_p0_y = p1.y-p0_y;
    	
    	int p2_p0_x = p2.x-p0_x;
    	int p2_p0_y = p2.y-p0_y;
    	
    	if(flag <0) {
    		p1.x = (int) (p0_x + 1.25*p1_p0_x);
    		p1.y = (int) (p0_y + 1.25*p1_p0_y);
    		p2.x = (int) (p0_x + 1.25*p2_p0_x);
    		p2.y = (int) (p0_y + 1.25*p2_p0_y);
    	}else if(flag>0) {
    		p1.x = (int) (p0_x + 0.8*p1_p0_x);
    		p1.y = (int) (p0_y + 0.8*p1_p0_y);
    		p2.x = (int) (p0_x + 0.8*p2_p0_x);
    		p2.y = (int) (p0_y + 0.8*p2_p0_y);
    	}
    }

	public abstract void draw(Graphics graphics);
	public abstract boolean isSelected(Point p);
    
}

class Point implements Serializable{
	
    public int x;
    public int y;
    Point(int xx,int yy)
    {
        x=xx;
        y=yy;
    }
}

class Line extends Shapes{  //线条
    Line(Point p1,Point p2)
    {
        super(p1, p2);
    }

    public void draw(Graphics g)
    {
        setLine(fine,g);
        g.setColor(color);
        g.drawLine(p1.x,p1.y,p2.x,p2.y);
    }


	public boolean isSelected(Point p)
    {
        if(p1.x>p2.x&&p1.y>p2.y)
        {
            return (p.x>p2.x&&p.x<(p1.x+p2.x)/2&&p.y>p2.y&&p.y<(p1.y+p2.y)/2)||
                    (p.x<p1.x&&p.x>(p1.x+p2.x)/2&&p.y<p1.y&&p.y>(p1.y+p2.y)/2);
        }
        else if(p2.x>p1.x&&p2.y>p1.y)
        {
            return (p.x>p1.x&&p.x<(p1.x+p2.x)/2&&p.y>p1.y&&p.y<(p1.y+p2.y)/2)||
                    (p.x<p2.x&&p.x>(p1.x+p2.x)/2&&p.y<p2.y&&p.y>(p1.y+p2.y)/2);
        }
        else if(p2.x>p1.x&&p2.y<p1.y)
        {
            return (p.x>p1.x&&p.x<(p1.x+p2.x)/2&&p.y<p1.y&&p.y>(p1.y+p2.y)/2)||
                    (p.x<p2.x&&p.x>(p1.x+p2.x)/2&&p.y>p2.y&&p.y<(p1.y+p2.y)/2);
        }
        else
        {
            return (p.x>p2.x&&p.x<(p1.x+p2.x)/2&&p.y<p2.y&&p.y>(p1.y+p2.y)/2)||
                    (p.x<p1.x&&p.x>(p1.x+p2.x)/2&&p.y>p1.y&&p.y<(p1.y+p2.y)/2);
        }
    }
}

class Square extends Shapes{  //矩形

    Square(Point p1,Point p2)
    {
        super(p1, p2);
    }

     
    public void draw(Graphics g)
    {
        setLine(fine,g);
        g.setColor(color);
        Point[] points=rectify();
        g.drawRect(points[0].x,points[0].y,points[1].x-points[0].x,points[1].y-points[0].y);
    }

     
    public boolean isSelected(Point p)
    {
        Point[] points=rectify();
        return p.x>points[0].x&&p.x<points[1].x&&p.y>points[0].y&&p.y<points[1].y;
    }
}

class Circle extends Shapes{  //圆

    Circle(Point p1,Point p2)
    {
        super(p1, p2);
    }
     
    public void draw(Graphics g)
    {
        setLine(fine,g);
        g.setColor(color);
        int d=Math.abs(p1.x-p2.x);
        Point[] points=rectify();
        g.drawOval(points[0].x,points[0].y,d,d);
    }
     
    public boolean isSelected(Point p)
    {
        Point[] points=rectify();
        return p.x>points[0].x&&p.x<points[1].x&&p.y>points[0].y&&p.y<points[1].y;
    }
}
