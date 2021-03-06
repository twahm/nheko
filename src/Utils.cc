#include "Utils.h"

#include <variant.hpp>

using TimelineEvent = mtx::events::collections::TimelineEvents;

QString
utils::descriptiveTime(const QDateTime &then)
{
        const auto now  = QDateTime::currentDateTime();
        const auto days = then.daysTo(now);

        if (days == 0)
                return then.toString("HH:mm");
        else if (days < 2)
                return QString("Yesterday");
        else if (days < 365)
                return then.toString("dd/MM");

        return then.toString("dd/MM/yy");
}

DescInfo
utils::getMessageDescription(const TimelineEvent &event,
                             const QString &localUser,
                             const QString &room_id)
{
        using Audio  = mtx::events::RoomEvent<mtx::events::msg::Audio>;
        using Emote  = mtx::events::RoomEvent<mtx::events::msg::Emote>;
        using File   = mtx::events::RoomEvent<mtx::events::msg::File>;
        using Image  = mtx::events::RoomEvent<mtx::events::msg::Image>;
        using Notice = mtx::events::RoomEvent<mtx::events::msg::Notice>;
        using Text   = mtx::events::RoomEvent<mtx::events::msg::Text>;
        using Video  = mtx::events::RoomEvent<mtx::events::msg::Video>;

        if (mpark::holds_alternative<Audio>(event)) {
                return createDescriptionInfo<Audio>(event, localUser, room_id);
        } else if (mpark::holds_alternative<Emote>(event)) {
                return createDescriptionInfo<Emote>(event, localUser, room_id);
        } else if (mpark::holds_alternative<File>(event)) {
                return createDescriptionInfo<File>(event, localUser, room_id);
        } else if (mpark::holds_alternative<Image>(event)) {
                return createDescriptionInfo<Image>(event, localUser, room_id);
        } else if (mpark::holds_alternative<Notice>(event)) {
                return createDescriptionInfo<Notice>(event, localUser, room_id);
        } else if (mpark::holds_alternative<Text>(event)) {
                return createDescriptionInfo<Text>(event, localUser, room_id);
        } else if (mpark::holds_alternative<Video>(event)) {
                return createDescriptionInfo<Video>(event, localUser, room_id);
        } else if (mpark::holds_alternative<mtx::events::Sticker>(event)) {
                return createDescriptionInfo<mtx::events::Sticker>(event, localUser, room_id);
        }

        return DescInfo{};
}

QString
utils::firstChar(const QString &input)
{
        if (input.isEmpty())
                return input;

        for (auto const &c : input.toUcs4()) {
                if (QString::fromUcs4(&c, 1) != QString("#"))
                        return QString::fromUcs4(&c, 1).toUpper();
        }

        return QString::fromUcs4(&input.toUcs4().at(0), 1).toUpper();
}

QString
utils::humanReadableFileSize(uint64_t bytes)
{
        constexpr static const char *units[] = {"B", "KiB", "MiB", "GiB", "TiB"};
        constexpr static const int length    = sizeof(units) / sizeof(units[0]);

        int u       = 0;
        double size = static_cast<double>(bytes);
        while (size >= 1024.0 && u < length) {
                ++u;
                size /= 1024.0;
        }

        return QString::number(size, 'g', 4) + ' ' + units[u];
}

int
utils::levenshtein_distance(const std::string &s1, const std::string &s2)
{
        const int nlen = s1.size();
        const int hlen = s2.size();

        if (hlen == 0)
                return -1;
        if (nlen == 1)
                return s2.find(s1);

        std::vector<int> row1(hlen + 1, 0);

        for (int i = 0; i < nlen; ++i) {
                std::vector<int> row2(1, i + 1);

                for (int j = 0; j < hlen; ++j) {
                        const int cost = s1[i] != s2[j];
                        row2.push_back(
                          std::min(row1[j + 1] + 1, std::min(row2[j] + 1, row1[j] + cost)));
                }

                row1.swap(row2);
        }

        return *std::min_element(row1.begin(), row1.end());
}

QString
utils::event_body(const mtx::events::collections::TimelineEvents &event)
{
        using namespace mtx::events;
        using namespace mtx::events::msg;

        if (mpark::holds_alternative<RoomEvent<Audio>>(event)) {
                return message_body<RoomEvent<Audio>>(event);
        } else if (mpark::holds_alternative<RoomEvent<Emote>>(event)) {
                return message_body<RoomEvent<Emote>>(event);
        } else if (mpark::holds_alternative<RoomEvent<File>>(event)) {
                return message_body<RoomEvent<File>>(event);
        } else if (mpark::holds_alternative<RoomEvent<Image>>(event)) {
                return message_body<RoomEvent<Image>>(event);
        } else if (mpark::holds_alternative<RoomEvent<Notice>>(event)) {
                return message_body<RoomEvent<Notice>>(event);
        } else if (mpark::holds_alternative<Sticker>(event)) {
                return message_body<Sticker>(event);
        } else if (mpark::holds_alternative<RoomEvent<Text>>(event)) {
                return message_body<RoomEvent<Text>>(event);
        } else if (mpark::holds_alternative<RoomEvent<Video>>(event)) {
                return message_body<RoomEvent<Video>>(event);
        }

        return QString();
}
