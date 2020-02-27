//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     quicktype::BossTimers data = nlohmann::json::parse(jsonString);

#pragma once

#include <nlohmann/json.hpp>

#include <optional>
#include <stdexcept>
#include <regex>

namespace quicktype {
    using nlohmann::json;

    inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }

    class BossTimersValue {
    public:
        BossTimersValue() = default;
        virtual ~BossTimersValue() = default;

    private:
        std::vector<std::string> monday;
        std::vector<std::string> tuesday;
        std::vector<std::string> wednesday;
        std::vector<std::string> thursday;
        std::vector<std::string> friday;
        std::vector<std::string> saturday;
        std::vector<std::string> sunday;

    public:
        const std::vector<std::string>& get_monday() const { return monday; }
        std::vector<std::string>& get_mutable_monday() { return monday; }
        void set_monday(const std::vector<std::string>& value) { this->monday = value; }

        const std::vector<std::string>& get_tuesday() const { return tuesday; }
        std::vector<std::string>& get_mutable_tuesday() { return tuesday; }
        void set_tuesday(const std::vector<std::string>& value) { this->tuesday = value; }

        const std::vector<std::string>& get_wednesday() const { return wednesday; }
        std::vector<std::string>& get_mutable_wednesday() { return wednesday; }
        void set_wednesday(const std::vector<std::string>& value) { this->wednesday = value; }

        const std::vector<std::string>& get_thursday() const { return thursday; }
        std::vector<std::string>& get_mutable_thursday() { return thursday; }
        void set_thursday(const std::vector<std::string>& value) { this->thursday = value; }

        const std::vector<std::string>& get_friday() const { return friday; }
        std::vector<std::string>& get_mutable_friday() { return friday; }
        void set_friday(const std::vector<std::string>& value) { this->friday = value; }

        const std::vector<std::string>& get_saturday() const { return saturday; }
        std::vector<std::string>& get_mutable_saturday() { return saturday; }
        void set_saturday(const std::vector<std::string>& value) { this->saturday = value; }

        const std::vector<std::string>& get_sunday() const { return sunday; }
        std::vector<std::string>& get_mutable_sunday() { return sunday; }
        void set_sunday(const std::vector<std::string>& value) { this->sunday = value; }
    };

    using BossTimers = std::map<std::string, quicktype::BossTimersValue>;
}

namespace quicktype {
    using BossTimers = std::map<std::string, quicktype::BossTimersValue>;
}

namespace nlohmann {
    namespace detail {
        void from_json(const json& j, quicktype::BossTimersValue& x);
        void to_json(json& j, const quicktype::BossTimersValue& x);

        inline void from_json(const json& j, quicktype::BossTimersValue& x) {
            x.set_monday(j.at("Monday").get<std::vector<std::string>>());
            x.set_tuesday(j.at("Tuesday").get<std::vector<std::string>>());
            x.set_wednesday(j.at("Wednesday").get<std::vector<std::string>>());
            x.set_thursday(j.at("Thursday").get<std::vector<std::string>>());
            x.set_friday(j.at("Friday").get<std::vector<std::string>>());
            x.set_saturday(j.at("Saturday").get<std::vector<std::string>>());
            x.set_sunday(j.at("Sunday").get<std::vector<std::string>>());
        }

        inline void to_json(json& j, const quicktype::BossTimersValue& x) {
            j = json::object();
            j["Monday"] = x.get_monday();
            j["Tuesday"] = x.get_tuesday();
            j["Wednesday"] = x.get_wednesday();
            j["Thursday"] = x.get_thursday();
            j["Friday"] = x.get_friday();
            j["Saturday"] = x.get_saturday();
            j["Sunday"] = x.get_sunday();
        }
    }
}
