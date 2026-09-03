#include <string>
#include <iostream>
#include <algorithm>

class Text
{
public:
    virtual void render(const std::string &data) const {
        std::cout << data;
    }
};


class DecoratedText : public Text
{
public:
    explicit DecoratedText(Text *text) : text_(text) {}
    Text *text_;
};

class Paragraph : public DecoratedText
{
public:
    explicit Paragraph(Text *text) : DecoratedText(text) {}

    void render(const std::string &data) const {
        std::cout << "<p>";
        text_->render(data);
        std::cout << "</p>";
    }
};

class Reversed : public DecoratedText
{
public:
    explicit Reversed(Text *text) : DecoratedText(text) {}

    void render(const std::string &data) const {
        auto rdata = data;
        std::reverse(rdata.begin(), rdata.end());

        text_->render(rdata);
    }
};

class Link : public DecoratedText
{
public:
    explicit Link(Text *text) : DecoratedText(text) {}

    void render(const std::string &link, const std::string &text) const {
        std::string data = "<a href =" + link + ">" + text + "</a>";
        text_->render(data);
    }
};

void task2_1()
{
    auto text_block1 = new Paragraph(new Text());
    text_block1->render("Hello world");
    std::cout << std::endl;

    auto text_block2 = new Reversed(new Text());
    text_block2->render("Hello world");
    std::cout << std::endl;

    auto text_block = new Link(new Text());
    text_block->render("netology.ru", "Hello world");
    std::cout << std::endl;
}
